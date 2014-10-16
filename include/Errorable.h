#ifndef D_ERRORABLE_H
#define D_ERRORABLE_H

#include <stdexcept>

template <typename ErrorType>
class Error {
private:
    ErrorType error_;

public:
    Error()
        : error_() {}

    explicit
    Error( const ErrorType& error )
        : error_(error) {}

    const ErrorType& what() const { return error_; }

    /* コピー・ムーブはpublic default属性 */
    Error(Error const&) = default;
    Error(Error &&) = default;
    Error& operator=(Error const&) = default;
    Error& operator=(Error &&) = default;

    /* デストラクタはpublic default */
    ~Error() = default;
};

template <typename ValueType, typename ErrorType=std::string>
class Errorable
{
private:
    bool valid_;
    ValueType value_;
    Error<ErrorType> error_;

public:
    Errorable()
        : valid_(true),
          value_(),
          error_() {}

    Errorable(const Error<ErrorType>& error)
        : valid_(false),
          value_(),
          error_(error) {}

    Errorable(const ValueType& value)
        : valid_(true),
          value_(value),
          error_() {}

    Errorable& operator=(const Error<ErrorType>& error)
    {
        valid_ = false;
        value_ = ValueType();
        error_ = error;
        return *this;
    }

    Errorable& operator=(const ValueType& value)
    {
        valid_ = true;
        value_ = value;
        error_ = Error<ErrorType>();
        return *this;
    }

    bool isError() const { return !valid_; }

    const ValueType& getValue() const
    {
        if(this->isError())
            throw std::logic_error("this is error object");

        return value_;
    }

    const Error<ErrorType>& getError() const
    {
        if(!this->isError())
            throw std::logic_error("this is value object");

        return error_;
    }

    /* コピー・ムーブはpublic default属性 */
    Errorable(Errorable const&) = default;
    Errorable(Errorable &&) = default;
    Errorable& operator=(Errorable const&) = default;
    Errorable& operator=(Errorable &&) = default;

    /* デストラクタはpublic default */
    ~Errorable() = default;
};

#endif
