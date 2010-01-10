

using namespace xml;



class MobFactory
{
public:
	MobFactory(const char* xml);
	Mob* generate();

private:
	void parse(document* xml);
	void parseOne(element* el);

	list<MobPrototype> l;
};

class MobPrototype
{
public:
	MobPrototype(int chance, range hp, range damage, int speed, IndexedList<graphic>* graphics);
	Mob* generate();

private:
	int chance;
	range hp;
	range damage;
	int speed;

	IndexedList<graphic>* graphics;
};

class Mob
{
	int hp, mhp;
	range damage;
	int speed;

	// Update on health change.
	graphic g;

	// Update from here.
	IndexedList<graphic>* graphics;
};

class IndexedList<class T>
{
	list<pair<int,T> > l;

	

}


Mob* MobFactory::generateMob()
{
	int percent = randInt(1, 100);

	for (el_it i = mobdoc->get_children(); *i; i++) {
		element* e = *i;
		printf("%s: found root element <%s>\n", fname, e->get_id());
	}
	
	for (int i = 1; i <= count; i++) {
		Str chanceStr(20);
		chanceStr += "Chance ";
		chanceStr += i;

		chance += mobs.getInt("Spawn", chanceStr);
		if (chance >= percent) {
			Str typeStr(10);
			typeStr += "Type ";
			typeStr += i;

			const char* type = mobs.getString("Spawn", typeStr);
			Mob* m = new Mob(type);
			return m;
		}
	}

	throw "Creating a non-existant mob not implimented yet";
}

Mob* MobPrototype::generate()
{
	Mob* m = new Mob();
}

MobFactory::MobFactory()
{
	document* xml = parse_xml("conf/mobs.xml");
	parse(xml);
	delete xml;
}

void MobFactory::parse(document* xml)
{
	for (el_it i = xml->get_children(); *i; i++) {
		element* e = *i;
		printf("Found mob: %s\n", e->get_id());
		parseOne(e);
	}
}



void MobFactory::parseOne(element* el)
{
	attribute
		*s_chance = el->get_attribute("chance"),
		*s_hp     = el->get_attribute("hp"),
		*s_damage = el->get_attribute("damage"),
		*s_speed  = el->get_attribute("speed");

	if (!s_chance || !s_hp || !s_damage || !s_speed)
		throw "Mob is missing attribute(s)";
	
	int chance = atoi(s_chance);
	range hp(s_hp);
	range damage(s_damage);
	int speed = atoi(s_speed);

	IndexedList<graphic>* graphics = new IndexedList<graphic>;
#error implement

	MobPrototype* mp = new MobPrototype(chance, hp, damage, speed, list);
}


