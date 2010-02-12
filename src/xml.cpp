/*
 * Version 1.2.2 @ 2009/11/7
 */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "e.h"
#include "xml.h"

namespace xml {

#define WHITESPACE_OR_END_OF_TAG "\t\n\r />"

/* node
 *
 * Internal linked list structure used to store children/attributes of
 * elements.
 */
struct node {
	node* next;
	void* data;
};

static node* create_node(void* datum) {
	node* n = new node;
	n->next = NULL;
	n->data = datum;

	return n;
}


/* reader
 *
 * Dedicated XML reader class. It's only purpose is to parse and XML document.
 * Used internally by the 'document' class.
 *
 * TODO: this class needs a complete makeover, it's organization is terrible
 */
class document::reader {
public:
	document* doc;
	element* par;
	char* buf;
	int i;
	int len;

	bool parse(document* d, char* buf);

private:
	inline char cur();
	inline char next();
	inline char next_char();
	inline char prev(int i);

	char* read_until(int bufsz, const char* delim,
		bool beware_escape = false);

	bool read_element();
	bool read_attribute();
	bool read_comment();
	bool read_end_tag();

	inline void skip_whitespace();
	inline bool is_whitespace();
	inline bool assert(char c);
};

/* writer
 *
 * Dedicated XML writer class. It's only purpose is to write out an XML
 * document.  Used internally by the 'document' class.
 */
class document::writer {
	FILE* f;
	int ind; // level of indentation

public:
	writer();
	~writer();

	bool to_file(document* doc, const char* fn);

private:
	void write_document(document* doc);
	void write_element(element* el);
	void write_attribute(attribute* at);

	void open_tag(element* el);
	void contained_tag(element* el);
	void close_tag(element* el);

	void indent();
};





////////////////////////////////////////
// class el_it

el_it::el_it(node* nod, const char* str)
		: n(nod) {
	if (str == NULL)
		strcpy(search, "");
	else
		strncpy(search, str, ELEMENT_ID_BUFSZ - 1);
}

element* el_it::operator *() {
	if (n)
		return (element*)n->data;
	return NULL;
}

void el_it::operator ++(int) {
	if (n && search[0] == '\0') {
		n = n->next;
		return;
	}
	while (n) {
		n = n->next;
		element* e = (element*)n->data;
		if (strcmp(e->get_id(), search) == 0)
			break;
	}
}





////////////////////////////////////////
// class at_it

at_it::at_it(node* nod) {
	n = nod;
}

attribute* at_it::operator *() {
	if (n)
		return (attribute*)n->data;
	return NULL;
}

void at_it::operator ++(int) {
	if (n)
		n = n->next;
}





////////////////////////////////////////
// class attribute

attribute::attribute(element* par, const char* Id, const char* val)
		: parent(par) {
	id = Id ? strdup(Id) : NULL;
	value = val ? strdup(val) : NULL;

	if (parent)
		parent->add(this);
}

attribute::~attribute() {
	if (id) free(id);
	if (value) free(value);
}

char* attribute::get_id() {
	return id;
}

char* attribute::get_value() {
	return value;
}

element* attribute::get_parent() {
	return parent;
}

void attribute::set_value(const char* val) {
	if (value)
		free(value);
	value = val ? strdup(val) : NULL;
}

void attribute::set_value_nodup(char* val) {
	if (value)
		free(value);
	value = val;
}





////////////////////////////////////////
// class element

element::element(element* par, const char* Id, const char* val)
		: attribute(NULL, Id, val),
		  first_at(NULL), last_at(NULL),
		  first_ch(NULL), last_ch(NULL) {
	parent = par;

	if (parent)
		parent->add(this);
}

element::~element() {
	while (has_attributes()) {
		node* n = first_at;
		first_at = first_at->next;

		attribute* a = (attribute*)n->data;
		delete a;

		delete n;
	}
	while (has_children()) {
		node* n = first_ch;
		first_ch = first_ch->next;

		element* e = (element*)n->data;
		delete e;

		delete n;
	}
}

void element::add(element* e) {
	if (e == NULL)
		return;

	node* n = create_node(e);
	if (has_children())
		last_ch->next = n;
	else
		first_ch = n;
	last_ch = n;
}

void element::add(attribute* a) {
	if (a == NULL)
		return;

	node* n = create_node(a);
	if (has_attributes())
		last_at->next = n;
	else
		first_at = n;
	last_at = n;
}

attribute* element::get_attribute(const char* id) {
	node* n = first_at;
	while (n) {
		attribute* a = (attribute*)n->data;
		if (strcmp(a->get_id(), id) == 0)
			return a;
		n = n->next;
	}
	return NULL;
}

at_it element::get_attributes() {
	at_it it(first_at);
	return it;
}

el_it element::get_children(const char* tag) {
	el_it it(first_ch, tag);
	return it;
}

bool element::has_attributes() {
	return first_at != NULL;
}

bool element::has_children() {
	return first_ch != NULL;
}





////////////////////////////////////////
// class document

document::document()
		: element(NULL, NULL, NULL) {
}

bool document::parse(char* buf) {
	reader r;
	return r.parse(this, buf);
}

bool document::write(const char* file) {
	writer w;
	return w.to_file(this, file);
}





////////////////////////////////////////
// class document::reader

inline char document::reader::cur() {
	return buf[i];
}

inline char document::reader::next() {
	return buf[i++];
}

inline char document::reader::next_char() {
	return buf[++i];
}

inline char document::reader::prev(int index) {
	return buf[i - index];
}

// TODO: rewrite to use buffers from alloca (hmm, is this linux only?)
char* document::reader::read_until(int bufsz, const char* delim,
		bool beware_escape) {
	int l = 0;
	int alloc = bufsz;
	char* s = (char*)malloc(bufsz);
	s[0] = 0;

	while (true) {
		const char* found = strchr(delim, cur());
		if (found != NULL) {
			if (!beware_escape || prev(1) != '\\') {
				s[l] = 0;
				return s;
			}
		}
		if (1 == alloc - l - 1) {
			alloc *= 3;
			s = (char*)realloc(s, alloc);
		}

		s[l++] = next();
	}
}

bool document::reader::parse(document* d, char* buffer) {
	doc = d;
	i = 0;
	par = doc;
	buf = buffer;

	while (true) {
		// we're not in an element, this is the root document
		if (par == doc) {
			while (true) {
				if (cur() == '\0' || cur() == '<')
					break;
				if (!is_whitespace()) {
					printf("illegal character "
						"outside of document... ");
					return false;
				}
				i++;
			}
		}
		// we are in an element!
		else {
			// Look for an element, add cdata while we find it
			char* cdata = read_until(CDATA_BUFSZ, "<");
			if (par->get_value() != NULL) // FIXME: better check
				par->set_value_nodup(cdata);
		}

		// End of file
		if (cur() == 0)
			break;

		// We've found a '<', what's the next char?
		switch (next_char()) {
			case '?':
				while (true) {
					char c = next_char();
					if (c == '>' && prev(1) == '?')
						break;
				}
				break;
			case '/':
				if (!read_end_tag()) return false;
				break;
			case '!':
				if (!read_comment()) return false;
				break;
			default:
				if (!read_element()) return false;
				break;
		}

		i++;

	}

	if (par != doc) {
		printf("ending while still in the middle of tag '%s'... ",
				par->get_id());
		return false;
	}
	return true;
}

bool document::reader::read_element() {
	skip_whitespace();

	// Get element tag
	char* elId = read_until(ELEMENT_ID_BUFSZ, WHITESPACE_OR_END_OF_TAG);
	if (strlen(elId) == 0) {
		printf("Element id length zero... ");
		return false;
	}

	element* e = new element(par, elId);
	free(elId);

	par = e;

	// Read attributes
	if (is_whitespace())
		while (read_attribute());

	// Element has no children
	if (cur() == '/') {
		par = e->get_parent();
	}

	else if (cur() != '/' && cur() != '>') {
		printf("Error reading end of open tag '%s'... ", e->get_id());
		return false;
	}

	i++;
	
	return true;
}

bool document::reader::read_attribute() {
	skip_whitespace();

	// End of element
	if (cur() == '/' || cur() == '>') {
		return false;
	}

	// Read attribute name
	char* atId = read_until(ATTRIBUTE_ID_BUFSZ, " \t\r\n=");

	if (strlen(atId) == 0) {
		printf("Attribute id length zero... ");
		return false;
	}

	skip_whitespace();
	if (!assert('=')) return false;
	i++;

	skip_whitespace();
	if (!assert('\"')) return false;
	i++;

	// Read attribute value
	char* atVal = read_until(ATTRIBUTE_VAL_BUFSZ, "\"", true);

	// create attribute
	new attribute(par, atId, atVal);

	free(atId);
	free(atVal);

	i++;

	return true;
}

bool document::reader::read_comment() {
	while (true) {
		if (cur() == 0)
			break;
		if ((prev(2) == '-') &&
		    (prev(1) == '-') &&
		    (cur() == '>'))
			break;
		i++;
	}

	return true;
}

bool document::reader::read_end_tag() {
	const char* id = par->get_id();

	i++;
	// TODO: read until whitespace
	char* end = read_until(ELEMENT_ID_BUFSZ, ">");

	if (id == NULL) {
		printf("Found an end tag '%s', but we're not in an "
			       "element...\n", end);
		return false;
	}

	if (strcmp(end, id)) {
		printf("Out of order end-tag '%s' should be '%s'... ",
				end, id);
		return false;
	}

	free(end);

	par = par->get_parent();
	return true;
}

inline void document::reader::skip_whitespace() {
	while (is_whitespace())
		i++;
}

/* is_whitespace
 *
 * Returns true if the current character is a whitespace character.
 */
inline bool document::reader::is_whitespace() {
	char c = cur();
	switch (c) {
		case ' ': case '\t':
	       	case '\n': case '\r':
			return true;
		default:
			return false;
	}
}

/* assert
 *
 * Asserts that the current character is what it should be.
 * Prints out a warning and returns false if it is not.
 */
inline bool document::reader::assert(char c) {
	if (cur() != c) {
		printf("Expecting '%c', got '%c'... ", c, cur());
		return false;
	}
	return true;
}





////////////////////////////////////////
// class document::writer

document::writer::writer() {
	f = NULL;
	ind = 0;
}

document::writer::~writer() {
}

bool document::writer::to_file(document* doc, const char* fn) {
	f = fopen(fn, "w");
	if (!f)
		return false;
	
	write_document(doc);

	fclose(f);
	return true;
}

void document::writer::write_document(document* doc) {
	for (el_it i = doc->get_children(); *i; i++)
		write_element(*i);
}

void document::writer::write_element(element* el) {
	if (el->has_children()) {
		indent();
		open_tag(el);
		fputc('\n', f);

		if (el->get_value()) {
			// This would be a really weird XML file...
			fputs(el->get_value(), f);
			fputc('\n', f);
		}

		ind++;
		for (el_it i = el->get_children(); *i; i++)
			write_element(*i);
		ind--;

		indent();
		close_tag(el);
		fputc('\n', f);
	}
	else if (el->get_value()) {
		indent();
		open_tag(el);

		fputs(el->get_value(), f);

		close_tag(el);
		fputc('\n', f);
	}
	else {
		indent();
		contained_tag(el);
		fputc('\n', f);
	}
}

void document::writer::write_attribute(attribute* at) {
	fputc(' ', f);
	fputs(at->get_id(), f);
	fputc('=', f);

	fputc('"', f);
	fputs(at->get_value(), f);
	fputc('"', f);
}

void document::writer::open_tag(element* el) {
	fputc('<', f);
	fputs(el->get_id(), f);

	for (at_it i = el->get_attributes(); *i; i++)
		write_attribute(*i);

	fputs(">", f);
}

void document::writer::contained_tag(element* el) {
	fputc('<', f);
	fputs(el->get_id(), f);

	for (at_it i = el->get_attributes(); *i; i++)
		write_attribute(*i);

	fputs("/>", f);
}

void document::writer::close_tag(element* el) {
	fputs("</", f);
	fputs(el->get_id(), f);
	fputc('>', f);
}

void document::writer::indent() {
	for (int i = 0; i < ind; i++)
		fputc('\t', f);
}

document* parse_xml(const char* filename) {
	char* buf = readFile(filename);
	if (!buf)
		err(1, "Couldn't read xml file");

	document* doc = new document;
	if (!doc->parse(buf))
		err(1, "Couldn't parse xml file");

	delete buf;
	return doc;
}


} // namespace xml

