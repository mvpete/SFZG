#ifndef __MEDIATOR_H__
#define __MEDIATOR_H__

#include <vector>
#include <string>

#include <memory>

class event_handler;

class event
{
public:
	virtual ~event() {};
	virtual void accept(event_handler &e) = 0;
};

class adjustment_event : public event
{
public:
	enum types { score, health };
private:
	int _modifier;
	types _t;
public:
	adjustment_event(types t, int modifier);
	void adjust(int &value);
	types& type() { return _t; }
	virtual void accept(event_handler &e);
};

class health_event : public adjustment_event
{
public:
	health_event(int modifier) : adjustment_event(adjustment_event::health, modifier)
	{
	}
};

class state_event : public event
{
public:
	enum types { new_game, quit, pause, resume, main_menu, lose };
	state_event(types t);
	virtual void accept(event_handler &e);
	types type();
private:
	types _t;

};

// this event is used to get values from an event_handler, stupid I know
// it is in-charge of destroying the payload
class payload_event : public event
{
	void *_payload;
	std::string _val;
public:
	payload_event(const std::string &value, void* payload);
	virtual void accept(event_handler &e);
	void*& payload();
	const std::string& id();
};

// must have a default handler for each event

class event_handler
{
	std::string _id;
public:
	event_handler(const std::string &id);
	const std::string& id();
	virtual ~event_handler() {};

	virtual void on_event(adjustment_event 	&evt) {};
	virtual void on_event(state_event 		&evt) {};
	virtual void on_event(payload_event 		&evt) {};
};

void apply_visitor(event_handler &handler, event &e);

class mediator
{
	static std::unique_ptr<mediator> instance_;
	std::vector<event_handler *> _handlers;
public:
	~mediator();
	static mediator* instance();
	void register_handler(event_handler *evt);
	void remove_handler(event_handler *evt);
	void remove_handler_by_id(const std::string &id);
	void send_event(const std::string &id, event *e);
};


#endif
