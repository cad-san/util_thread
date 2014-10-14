#ifndef D_ERRORABLE_H
#define D_ERRORABLE_H

template <typename ErrorType>
class Error {
private:
    const ErrorType error_;

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
    explicit
    Errorable(const Error<ErrorType>& error)
        : valid_(false),
          value_(),
          error_(error) {}

    explicit
    Errorable(const ValueType& value)
        : valid_(true),
          value_(value),
          error_() {}

    bool isError() const { return !valid_; }

    ValueType getValue() const { return value_; }

    Error<ErrorType> getError() const { return error_; }
};

#endif
