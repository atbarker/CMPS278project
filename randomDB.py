import sqlite3
import random

def choose_class():
	rand = random.randint(1, 9)
	cls = " "
	if(rand == 1):
		cls = "fighter"
	if(rand == 2):
		cls = "barbarian"
	if(rand == 3):
		cls = "bard"
	if(rand == 4):
		cls = "cleric"
	if(rand == 5):
		cls = "druid"
	if(rand == 6):
		cls = "paladin"
	if(rand == 7):
		cls = "ranger"
	if(rand == 8):
		cls = "wizard"
	else:
		cls = "bort"
	return cls

def choose_name():
	rand = random.randint(1, 5)
	name = "bort"
	if(rand == 1):
		name = "brian"
	if(rand == 2):
		name = "dan"
	if(rand == 3):
		name = "bob"
	if(rand == 4):
		name = "stanley"
	else:
		name = "4"
	return name

def random_read(conn):
	c = conn.cursor()
	c.execute('''SELECT * FROM characters''')
	conn.commit()

def random_insert(conn):
	cls = choose_class()
	name = choose_name()
	hp = random.randint(1, 30)
	lvl = random.randint(1, 21)
	c = conn.cursor()
	character = (cls, name, hp, "yes", lvl)
	c.execute("INSERT INTO characters VALUES (?,?,?,?,?)", character)
	conn.commit()

def random_update(conn):
	cls = choose_class()
	name = choose_name()
	hp = random.randint(1, 30)
	lvl = random.randin(1, 21)
	c = conn.cursor()
	c.execute("UPDATE characters SET hp = %d WHERE name = %s" % (hp, name))
	conn.commit()
	

def main_loop():
	conn = sqlite3.connect('testing.db')
	print "Connection established"
	c = conn.cursor()
	c.execute('''CREATE TABLE characters (name text, class text, hp real, alive text, lvl real)''')
	conn.commit()
	while():
		ran = random.randint(1, 4)
		if (ran == 1):
			random_read(conn)
		else if(ran == 2):
			random_insert(conn)
		else:
			random_update(conn)
		
	conn.close()
	
if __name__ == "__main__":
	main()
