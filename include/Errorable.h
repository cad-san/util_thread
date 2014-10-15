#ifndef D_ERRORABLE_H
#define D_ERRORABLE_H

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

    const ErrorType& get() const { return error_; }
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

    const ValueType& getValue() const { return value_; }
    const Error<ErrorType>& getError() const { return error_; }
};

#endif
