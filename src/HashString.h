#ifndef HASH_STRING_H
#define HASH_STRING_H

#include <string>
#include <functional>
#include <map>

/// Unique String Identifier
typedef unsigned int StringID;

/** \brief String for quick comparisons and copying.
 *  HashString uses a hash function to associate a semi-unique unsigned int id
 *  for a string.  This string is then interned, or added to a table of strings
 *  with the unique hash value as its key.  Comparrisons to other HashStrings
 *  use this id to compare, instead of the string itself, resulting in O(1)
 *  comparison times instead of O(n).
 *  HashStrings can later be created using this unique id, given that a
 *  string with that ID has already been interned.
 *
 *  How to Use:
 *
 *  Bad:
 *  \code
 *	HashString event_type = event.getEventType();
 *	if ( event_type == HashString( "PlayerMove" ) )	// O(n)
 *	{
 *		...
 *	}
 *	else if ( event_type == HashString( "PlayerDie" ) )	// O(n)
 *	{
 *		...
 *	}
 *	\endcode
 *	Creating HashStrings will call the hash function each time, which is taxing and defeats the purpose
 *	of HashStrings.
 *
 *	Good:
 *	\code
 *	HashString const playerMoveStr( "PlayerMove" );
 *	HashString const playerDie( "PlayerMove" );
 *	...
 *	HashString event_type = event.getEventType();
 *	if ( event_type == playerMoveStr )	// O(1)
 *	{
 *		...
 *	}
 *	else if ( event_type == playerDie )	// O(1)
 *	{
 *		...
 *	}
 *	\endcode
 *	Comparing to previously made HashStrings or directly to the StringID is quick, clean and efficent.
 *	I tell you what.
 *	Create Const HashString early on in code and reference those directly.
 */
class HashString
{
friend class HashStringInitilizer;
// # Static Region

private:
    /// Interned String Map Type
    typedef std::map< StringID, std::string const > InternStringMap;
    /// Interned String Insertion Type
    typedef std::pair< StringID, std::string const > InternStringPair;
    /// Iterator for Interned String Map
    typedef InternStringMap::iterator InternStringMapIter;
    /// Const Iterator for Interned String Map
    typedef InternStringMap::const_iterator InternStringMapConstIter;

	/// Interned String map
    static InternStringMap * s_internedStrings;

    /// The hash function
    static std::hash<std::string> * s_stringHash;

public:

	/** \brief Returns true if string is already interned.
      * \param str String to check for
      * \return True if string is already interned.
      */
	static bool isStringInterned( std::string const & str );

	/** \brief Returns true if string is already interned.
      * \param hash_value StringID to check for
      * \return True if string is already interned.
      */
	static bool isStringInterned( StringID const & hash_value );

	/** \brief Interns the string for future use.
	  * \param str String to intern
	  * \return String ID this string is linked to.
	  * \note Interning an already intern string will have no
	  *     negative effect. It will simply return the existing
	  *     ID, but not add it.
	  */
    static StringID internString( std::string const & str );

	static std::string getStringFromHash( StringID const & id );
	
	static std::map< StringID, std::string const > getInternMap() { return *s_internedStrings; }

// # End of Static Region

private:

    /// Iterator to the position of this entry in the map
    InternStringMapConstIter m_mapPosition;

public:

	/// Returns string value
	std::string getString() const;

	/// Returns string hash value
	StringID getHashValue() const;

	HashString();

	HashString( HashString const & other );

    /** \brief Constructor that creates and ( if it doesn't exist ) adds to the interned string map
     *  Constructor that creates and ( if it doesn't exist ) adds to the interned string map.
     * \param str String Value
     */
    explicit HashString( std::string const & str );

    /** \brief Constructor for an already internned string, with this ID
     *  Constructor that takes in the string Id, and finds it's string value
	 *  for this HashString.  Will throw UninternedStringIDRefException if string
	 *  ID is not yet internned.
     *  \param str_id Hash String ID for the look up
     */
    explicit HashString( StringID const & str_id );

	HashString( char const * c_str );

    virtual ~HashString();

	// # Operators
	HashString & operator=( HashString const & other );

	/// Less than operator ( uses m_hashValue )
	bool operator< ( HashString const & other ) const;

	/// Equality operator ( uses m_hashValue )
	bool operator== ( HashString const & other ) const;
	bool operator!= ( HashString const & other ) const;

	bool operator== ( std::string const & other ) const;
	bool operator!= ( std::string const & other ) const;
	bool operator== ( StringID const & other ) const;
	bool operator!= ( StringID const & other ) const;

	bool operator< ( StringID const & other ) const;

//    operator StringID const & () const;

	// Const
	static HashString const s_kEmptyString;
};

/// This class is used for the static member initilization
static class HashStringInitilizer
{
public:
    HashStringInitilizer();
    ~HashStringInitilizer();

    static int getSchwarz();
} initializer;

#endif
