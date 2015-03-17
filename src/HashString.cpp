#include "HashString.h"
#include <iostream>
#include <cassert>

using namespace std;

HashString::InternStringMap * HashString::s_internedStrings;
std::hash<std::string> * HashString::s_stringHash;

/// Static Counter
static int s_schwarzCounter = 0;

int HashStringInitilizer::getSchwarz()
{
    return s_schwarzCounter;
}

/// Static Initializer of HashString table and hash function
HashStringInitilizer::HashStringInitilizer()
{
    if ( s_schwarzCounter == 0 )
    {
        HashString::s_internedStrings = new HashString::InternStringMap();
        HashString::s_stringHash = new std::hash<std::string>();

        //cout << "inited\n";

        s_schwarzCounter++;
    }
}

/// Static Deconstructor of HashString table and hash function
HashStringInitilizer::~HashStringInitilizer()
{
    if ( --s_schwarzCounter == 0 )
	{
		delete HashString::s_internedStrings;
		delete HashString::s_stringHash;
	}
}

HashString const HashString::s_kEmptyString("");

/// Returns true if string is already interned
bool HashString::isStringInterned( std::string const & str )
{
	// Hash it's value, find if that is key in map
	StringID hash_value = (*s_stringHash)( str );

	if ( s_internedStrings->find( hash_value ) != s_internedStrings->cend() )
	{
		return true;
	}

	return false;
}

bool HashString::isStringInterned( StringID const & hash_value )
{
	// Hash it's value, find if that is key in map
	if ( s_internedStrings->find( hash_value ) != s_internedStrings->cend() )
	{
		return true;
	}

	return false;
}

/// Interns the string for future use
StringID HashString::internString( std::string const & str )
{
	StringID hash_value = (*s_stringHash)( str );

	/// If we are able to find it, return the hash_value
	if ( s_internedStrings->find( hash_value ) != s_internedStrings->cend() )
	{
		return hash_value;
	}

	// Add string to interned table
	s_internedStrings->insert( InternStringPair( hash_value, str ) );

	return hash_value;
}

std::string HashString::getStringFromHash( StringID const & id )
{
	std::string rval;

	auto iter = s_internedStrings->find( id );

	if ( iter != s_internedStrings->cend() )
	{
		rval = (*iter).second;
	}

	return rval;
}

// # End of Static Region

/// Returns string value
std::string HashString::getString() const
{
	return m_mapPosition->second;
}

HashString::HashString()
:	HashString( s_kEmptyString )	// redirect to copy constructor
{
}

HashString::HashString( HashString const & other )
:	m_mapPosition( other.m_mapPosition ),
	m_hashValue( other.m_hashValue )
{
}

/// Constructor that creates and ( if it doesn't exist ) adds to the interned string map
HashString::HashString( std::string const & str )
{
	m_hashValue = (*s_stringHash)( str );

    // Insert doesn't care if it already exists
    std::pair< InternStringMapIter, bool > result =
		s_internedStrings->insert( InternStringPair( m_hashValue, str ) );

    m_mapPosition = result.first;
}

/// Constructor that takes in the string Id, and finds it's string value
/// for this HashString.  Will throw UninternedStringIDRefException if string
/// ID is not yet internned
HashString::HashString( StringID const & str_id )
:	m_hashValue( str_id )
{
    // Find this key in the map
    m_mapPosition = s_internedStrings->find( str_id );

    // it it doesn't exist, complain, loudly
    if ( m_mapPosition == s_internedStrings->cend() )
    {
		assert ( 0 && "Uninterned HashString Referenced" );
    }
}

HashString::HashString( char const * c_str )
:	HashString( std::string( c_str ) )
{
}

HashString::~HashString()
{
}

// # Operators

HashString & HashString::operator=( HashString const & other )
{
	this->m_mapPosition = other.m_mapPosition;
	m_hashValue = other.m_hashValue;

	return *this;
}

bool HashString::operator< ( HashString const & other ) const
{
	return ( m_hashValue < other.m_hashValue );
}

bool HashString::operator== ( HashString const & other ) const
{
	return ( m_hashValue == other.m_hashValue );
}

bool HashString::operator!= ( HashString const & other ) const
{
	return !( m_hashValue == other.m_hashValue );
}

bool HashString::operator== ( std::string const & other ) const
{
	return ( getString() == other );
}

bool HashString::operator!= ( std::string const & other ) const
{
	return ! ( getString() == other );
}

bool HashString::operator== ( StringID const & other ) const
{
	return ( m_hashValue == other );
}

bool HashString::operator!= ( StringID const & other ) const
{
	return ( m_hashValue != other );
}

bool HashString::operator< ( StringID const & other ) const
{
	return ( m_hashValue < other );
}

//HashString::operator StringID const & () const
//{
//    return m_mapPosition->first;
//}
