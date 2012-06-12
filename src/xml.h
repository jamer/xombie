#ifndef XML_H
#define XML_H

#define CDATA_BUFSZ 1024
#define ELEMENT_ID_BUFSZ 32
#define ATTRIBUTE_ID_BUFSZ 32
#define ATTRIBUTE_VAL_BUFSZ 512

// TODO: rename id to "tag" or "value"

namespace xml {

struct node;
class attribute;
class element;
class document;

/* el_it
 *
 * XML element iterator. This is what you use to read an element's children.
 * Give it a search string, and it will iterate through all the children with
 * that tag.  If search is NULL, iterate through all children.
 */
class el_it {
public:
	el_it(node* n, const char* search);

	element* operator *();
	void operator ++(int);
private:
	node* n;
	char search[ELEMENT_ID_BUFSZ];
};

/* at_it
 *
 * XML attribute iterator. This is what you use to read an element's attributes.
 */
class at_it {
public:
	at_it(node* n);

	attribute* operator *();
	void operator ++(int);
private:
	node* n;
};

class attribute {
public:
	attribute(element* parent, const char* id, const char* value);
	~attribute();

	char* get_id();
	char* get_value();
	element* get_parent();

	// set value, strdup'ing
	void set_value(const char* value);

	// set value directly without a strdup
	void set_value_nodup(char* value);

protected:
	char* id;
	char* value;
	element* parent;
};

class element: public attribute {
public:
	element(element* parent, const char* id, const char* value = NULL);
	~element();

	/* add
	 *
	 * Adds an attribute or a child element to our list of such. Does not
	 * check for duplicate entries.
	 */
	void add(element* e);
	void add(attribute* a);

	/* get_attribute
	 *
	 * Returns the first attribute found with id of "id." If more than one
	 * attribute exists with this id, only the first will be returned.
	 * Gives NULL if not no attribute was found.
	 */
	attribute* get_attribute(const char* id = NULL);

	at_it get_attributes();

	/* get_children
	 *
	 * Returns an XML iterator which can be used to iterate through the
	 * list of children with the specified tag name.
	 */
	el_it get_children(const char* tag = NULL);

	bool has_attributes();
	bool has_children();

private:
	// Linked list of the attributes and children.
	node* first_at, *last_at;
	node* first_ch, *last_ch;
};

/* document
 *
 * A document is basically a glorified top level element. It has methods that
 * can translate the data between a character-based XML document and the
 * internal DOM structure that your program manipulates.
 */
class document: public element {
public:
	document();

	// Methods
	bool parse(const char* buf);
	bool write(const char* file);

	// Members
private:

	class reader;
	class writer;
};

document* parse_xml(const char* filename);

#endif // XML_H

} // namespace xml
