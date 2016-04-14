#include "mediator.h"
#include <algorithm>
#include <iostream>


/////////////////////////////////////////////////////
// events
adjustment_event::adjustment_event(types t, int modifier)
	:_modifier(modifier), _t(t)
{
}

void adjustment_event::adjust(int &value)
{
	value += _modifier;
}

void adjustment_event::accept(event_handler &e)
{
	e.on_event(*this);
}

///
state_event::state_event(types t)
	:_t(t)
{
}

state_event::types state_event::type()
{
	return _t;
}

void state_event::accept(event_handler &e)
{
	e.on_event(*this);
}

///
payload_event::payload_event(const std::string &id, void *payload)
	:_val(id), _payload(payload)
{
}


void *&payload_event::payload()
{
	return _payload;
}

void payload_event::accept(event_handler &e)
{
	e.on_event(*this);
}
const std::string& payload_event::id()
{
	return _val;
}
/////////////////////////////////////////////////////
/// event_handler

event_handler::event_handler(const std::string &id) :_id(id) {}
const std::string& event_handler::id() { return _id; }


void apply_visitor(event_handler &handler, event &e)
{
	e.accept(handler);
}


struct find_handler_by_id
{
	std::string _id;
public:
	find_handler_by_id(const std::string &id) :_id(id) {}
	bool operator()(event_handler* e)
	{
		return _id.compare(e->id()) == 0;
	}
};


std::unique_ptr<mediator> mediator::instance_ = NULL;

mediator * mediator::instance()
{
	if (instance_ == NULL)
		instance_.reset(new mediator());
	return instance_.get();
}

mediator::~mediator()
{
	std::cout << "deleting mediator" << std::endl;
}

void mediator::register_handler(event_handler *evt)
{
	_handlers.push_back(evt);
}

void mediator::remove_handler(event_handler *evt)
{
	_handlers.erase(std::remove(_handlers.begin(), _handlers.end(), evt));
}

/// had to use this because basically calling remove_handler in the destructor was calling virtual functions on objects that just didn't exist
void mediator::remove_handler_by_id(const std::string &id)
{
	_handlers.erase(std::remove_if(_handlers.begin(), _handlers.end(), find_handler_by_id(id)), _handlers.end());

}

void mediator::send_event(const std::string &id, event *e)
{
	std::vector<event_handler*>::iterator i = std::find_if(_handlers.begin(), _handlers.end(), find_handler_by_id(id));
	if (i == _handlers.end())
	{
		std::cout << "no handler named: " << id << std::endl;
		return;
	}

	apply_visitor(*(*i), *e);
}
