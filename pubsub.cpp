#include <vector>
#include <utility>
#include <string>
#include <unordered_map>
#include <list>
#include <iostream>

class Channel
{
	friend class Server;

	const std::string name;
	double price;

public:
	Channel(std::string &&n, double p) : name(std::move(n)), price(p){};
	const std::string &get_name() const { return name; }
};

class Subscriber
{
	friend class Server;

	const std::string name;

public:
	Subscriber(std::string &&n) : name(std::move(n)){};
	const std::string &get_name() const { return name; }

	bool operator==(const Subscriber &s) const { return name == s.get_name(); }

	void send_update(Channel *c, double newPrice)
	{
		std::cout << "Price of " << c->get_name() << " has been updated " << std::endl;
	}
};

class Server
{
	std::unordered_map<Subscriber *, std::list<Channel *>> subscriptionsMap;
	std::unordered_map<Channel *, std::list<Subscriber *>> subscribersMap;

public:
	Server() = default;
	bool add_subscription(Subscriber *s, Channel *c)
	{
		subscribersMap[c].emplace_front(s);
		subscriptionsMap[s].emplace_front(c);
		return true;
	}

	void update_price(Channel *c, double newPrice)
	{
		c->price = newPrice;
		for (Subscriber *s : subscribersMap[c])
			s->send_update(c, newPrice);
	}
};

int main()
{
	return 0;
}
