#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class RentalException : public std::runtime_error {
public:
    explicit RentalException(const std::string& message) : std::runtime_error(message) {}
};

class AuthException : public RentalException {
public:
    explicit AuthException(const std::string& message) : RentalException(message) {}
};

class ValidationException : public RentalException {
public:
    explicit ValidationException(const std::string& message) : RentalException(message) {}
};

class NotFoundException : public RentalException {
public:
    explicit NotFoundException(const std::string& message) : RentalException(message) {}
};

#endif // EXCEPTIONS_H
