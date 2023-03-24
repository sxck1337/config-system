#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <algorithm>

template<class>
struct always_false : std::false_type {};

template<class T>
inline constexpr bool always_false_v = always_false<T>::value;

class Config {
public:
    template<typename T>
    void Set( const std::string& key, const T& value ) {
        values_[key] = ConfigValue{ value };
    }

    template<typename T>
    T Get( const std::string& key ) const {
        return values_.at( key ).GetValue<T>( );
    }

    void SaveToFile( const std::string& filename ) const {
        std::ofstream file( filename );
        if (!file.is_open( )) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            return;
        }
        for (const auto& [key, value] : values_) {
            file << key << " = " << value.ToString( ) << std::endl;
        }
    }
    void LoadFromFile( const std::string& filename ) {
        std::ifstream file( filename );
        if (!file.is_open( )) {
            std::cerr << "Failed to open file for reading: " << filename << std::endl;
            return;
        }
        std::string line;
        while (std::getline( file, line )) {
            size_t pos = line.find( '=' );
            if (pos != std::string::npos) {
                std::string key = line.substr( 0, pos );
                std::string value = line.substr( pos + 1 );
             
                key.erase( 0, key.find_first_not_of( " \t" ) );
                key.erase( key.find_last_not_of( " \t" ) + 1 );
                value.erase( 0, value.find_first_not_of( " \t" ) );
                value.erase( value.find_last_not_of( " \t" ) + 1 );
                if (values_.count( key ) == 0) {
                    values_[key] = ConfigValue{};
                }
                values_[key].FromString( value );
                std::cout << key << " = " << value << std::endl;
            }
        }
    }



    class Subcategory {
    public:
        Subcategory( Config& parent, const std::string& name ) : parent_( parent ), name_( name ) {}

        template<typename T>
        void Set( const std::string& key, const T& value ) {
            parent_.Set( name_ + "." + key, value );
        }

        template<typename T>
        T Get( const std::string& key ) const {
            return parent_.Get<T>( name_ + "." + key );
        }

        Subcategory GetSubcategory( const std::string& name ) const {
            return Subcategory( parent_, name_ + "." + name );
        }

    private:
        Config& parent_;
        std::string name_;
    };

    Subcategory GetSubcategory( const std::string& name ) {
        return Subcategory( *this, name );
    }

private:
    struct ConfigValue {

        ConfigValue( ) = default;

        template<typename T>
        ConfigValue( const T& value ) {
            SetValue( value );
        }

        template<typename T>
        void SetValue( const T& value ) {
            if constexpr (std::is_same_v<T, bool>) {
                data_ = value ? "true" : "false";
            }
            else {
                data_ = std::to_string( value );
            }
        }

        void SetValue( const char* value ) {
            data_ = std::string( value );
        }

        template<typename T>
        T GetValue( ) const {
            if constexpr (std::is_same_v<T, int>) {
                return std::stoi( data_ );
            }
            else if constexpr (std::is_same_v<T, float>) {
                return std::stof( data_ );
            }
            else if constexpr (std::is_same_v<T, double>) {
                return std::stod( data_ );
            }
            else if constexpr (std::is_same_v<T, bool>) {
                if (data_ == "true") {
                    return true;
                }
                else if (data_ == "false") {
                    return false;
                }
                else if (data_ == "1") {
                    return true;
                }
                else if (data_ == "0") {
                    return false;
                }
                else {
                    throw std::invalid_argument( "Invalid bool value: " + data_ );
                }
            }
            else if constexpr (std::is_same_v<T, std::string>) {
                return data_;
            }
            else if constexpr (std::is_same_v<T, char>) {
                return data_[0];
            }
            else if constexpr (std::is_same_v<T, unsigned long long int>) {
                return std::stoull( data_ );
            }
            else {
                static_assert(always_false_v<T>, "Unsupported type");
            }
        }

        void FromString( const std::string& str ) {
            data_ = str;
        }

        std::string ToString( ) const {
            return data_;
        }

    private:
        std::string data_;
    };

    std::unordered_map<std::string, ConfigValue> values_;
} inline config;

