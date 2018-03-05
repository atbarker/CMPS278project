import sqlite3
import random
import time

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
	return name

def random_read(conn):
	c = conn.cursor()
	c.execute('''SELECT * FROM characters''')
	conn.commit()

def random_insert(conn, index):
	cls = choose_class()
	name = choose_name()
	hp = random.randint(1, 30)
	lvl = random.randint(1, 21)
	try:
		c = conn.cursor()
		character = (index, name, cls, hp, "yes", lvl)
		c.execute("INSERT INTO characters VALUES (?,?,?,?,?,?)", character)
		conn.commit()
	except:
		print "Insert failed"

def random_update(conn):
	cls = choose_class()
	name = choose_name()
	hp = random.randint(1, 30)
	lvl = random.randint(1, 21)
	try:
		c = conn.cursor()
		c.execute("UPDATE characters SET hp = %d WHERE name = %s" % (hp, name))
		conn.commit()
	except:
		print "Update failed"
	

def main_loop():
	characterIndex = 1
	conn = sqlite3.connect('testing.db')
	print "Connection established"
	c = conn.cursor()
	c.execute('''CREATE TABLE IF NOT EXISTS characters (id real, name text, class text, hp real, alive text, lvl real)''')
	conn.commit()
	conn.execute('pragma journal_mode=wal')
	while True:
		ran = random.randint(1, 4)
		if (ran == 1):
			random_read(conn)
			print "Random read"
		elif(ran == 2):
			random_insert(conn, characterIndex)
			print "Random insert"
			characterIndex += 1
		else:
			random_update(conn)
			print "Random update"
		time.sleep(5)
	conn.close()
	
if __name__ == "__main__":
	main_loop()
