#include <functional> 
#include <type_traits>

//프로퍼티 사용시 1회 포함
#define USING_PROPERTY(class_name) \
using OwnerType = class_name;

#define GETTER(field_name)                                                                     \
private:                                                                                       \
struct field_name##_property_getter_struct                                                     \
{                                                                                              \
    decltype(field_name)& operator()(OwnerType* _this) const                                   \
    {                                                                                          \
        return _this->field_name##_property_getter();                                          \
    }                                                                                          \
};                                                                                             \
using field_name##_property_getter_t = field_name##_property_getter_struct;                    \
decltype(field_name)& field_name##_property_getter()                                         
                                                                                               
#define SETTER(field_name)                                                                   \
private:                                                                                       \
struct field_name##_property_setter_struct                                                     \
{                                                                                              \
    void operator()(OwnerType* _this, const decltype(field_name)& value)      \
    {                                                                                          \
        _this->field_name##_property_setter(value);                                     \
    }                                                                                          \
};                                                                                             \
using field_name##_property_setter_t = field_name##_property_setter_struct;                    \
void field_name##_property_setter(const decltype(field_name)& value)

#define GETTER_ONLY(field_name)                                                                \
private:                                                                                       \
using field_name##_property_setter_t = void;                                                   \
GETTER(field_name)               

#define SETTER_ONLY(field_name)                                                                \
private:                                                                                       \
using field_name##_property_getter_t = void;                                                   \
SETTER(field_name)   

#define PROPERTY(field_type, field_name, property_name)                                                                             \
public:                                                                                                                             \
    TProperty<OwnerType, field_type, field_name##_property_getter_t, field_name##_property_setter_t> property_name{ this };  

//Set, Get 함수 선언 도움을 위한 헬퍼 템플릿 클래스
template <typename owner_type, typename field_type, class getter = void, class setter = void>
class TProperty
{
    static constexpr bool is_getter = !std::is_same_v<getter, void>;
    static constexpr bool is_setter = !std::is_same_v<setter, void>;
    static_assert(is_getter || is_setter, "TProperty must have either a getter or a setter.");
public:
    TProperty(
        owner_type* _this
    ) 
        :
        owner(_this)
    {
        
    }

private:
    owner_type* owner = nullptr;

    using getterType = std::conditional_t<is_getter, getter, char>;
    using setterType = std::conditional_t<is_setter, setter, char>;

    getterType get{};
    setterType set{};

    field_type& Getter() const
    {
        return get(owner);
    }
    void Setter(const field_type& rhs)
    {
        set(owner, rhs);
    }

public:
    //Read
    inline operator const field_type& () const requires(is_getter)
    { 
        return this->Getter();
    }

    inline const field_type& operator->() requires ( std::is_pointer_v<owner_type>&& is_getter)
    { 
        return  this->Getter();
    }
    inline const field_type* operator->() requires (!std::is_pointer_v<owner_type>&& is_getter)
    { 
        return &this->Getter();
    }

    //Write
    inline TProperty& operator=(const TProperty& rhs) requires (is_setter)
    {
        if (this != &rhs)
        {
            this->Setter(rhs.Getter());
        }
        return *this;
    }
    inline TProperty& operator=(const field_type& rhs) requires (is_setter)
    {
        this->Setter(rhs);
        return *this;
    }

    inline TProperty& operator+=(const TProperty& rhs) requires (is_setter)
    {
        if (this != &rhs)
        {
            this->Setter(this->Getter() + rhs.Getter());
        }
        return *this;
    }
    inline TProperty& operator+=(const field_type& rhs) requires (is_setter)
    {
        this->Setter(this->Getter() + rhs);
        return *this;
    }

    inline TProperty& operator-=(const TProperty& rhs) requires (is_setter)
    {
        if (this != &rhs)
        {
            this->Setter(this->Getter() - rhs.Getter());
        }
        return *this;
    }
    inline TProperty& operator-=(const field_type& rhs) requires (is_setter)
    {
        this->Setter(this->Getter() - rhs);
        return *this;
    }

    inline TProperty& operator*=(const TProperty& rhs) requires (is_setter)
    {
        if (this != &rhs)
        {
            this->Setter(this->Getter() * rhs.Getter());
        }
        return *this;
    }
    inline TProperty& operator*=(const field_type& rhs) requires (is_setter)
    {
        this->Setter(this->Getter() * rhs);
        return *this;
    }

    inline TProperty& operator/=(const TProperty& rhs) requires (is_setter)
    {
        if (this != &rhs)
        {
            this->Setter(this->Getter() / rhs.Getter());
        }
        return *this;
    }
    inline TProperty& operator/=(const field_type& rhs) requires (is_setter)
    {
        this->Setter(this->Getter() / rhs);
        return *this;
    }

};

