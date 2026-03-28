#include "subsystems/Pneumatic.h"
#include "commands/ButtonCommand.h"
#include <memory>

// ─── Single Acting ────────────────────────────────────────────────────────────

SingleActingPneumatic::SingleActingPneumatic(int pistonPort, bool startExtended)
	: piston(pistonPort, startExtended), extended(startExtended) {}

void SingleActingPneumatic::setup() {
	if (extended) extend(); else retract();
}

void SingleActingPneumatic::registerHandlers(EventLoop &eventLoop) {
	if (!hasHandler) return;
	auto cb = callback ? callback : [this]() { toggle(); };
	eventLoop.registerCommand(
		std::unique_ptr<Command>(new ButtonCommand(
			eventLoop.controller, button, cb
		))
	);
}

void SingleActingPneumatic::extend() {
	piston.extend();
	extended = true;
}

void SingleActingPneumatic::retract() {
	piston.retract();
	extended = false;
}

void SingleActingPneumatic::toggle() {
	if (extended) retract(); else extend();
}

bool SingleActingPneumatic::isExtended() const { return extended; }

SingleActingPneumatic &SingleActingPneumatic::withHandler(
	pros::controller_digital_e_t btn, std::function<void()> cb)
{
	button = btn;
	callback = cb;
	hasHandler = true;
	return *this;
}

// ─── Double Acting ────────────────────────────────────────────────────────────

DoubleActingPneumatic::DoubleActingPneumatic(int extendPort, int retractPort, bool startExtended)
	: extendPiston(extendPort, startExtended),
	  retractPiston(retractPort, !startExtended),
	  extended(startExtended) {}

void DoubleActingPneumatic::setup() {
	if (extended) extend(); else retract();
}

void DoubleActingPneumatic::registerHandlers(EventLoop &eventLoop) {
	if (!hasHandler) return;
	auto cb = callback ? callback : [this]() { toggle(); };
	eventLoop.registerCommand(
		std::unique_ptr<Command>(new ButtonCommand(
			eventLoop.controller, button, cb
		))
	);
}

void DoubleActingPneumatic::extend() {
	extendPiston.extend();
	retractPiston.retract();
	extended = true;
}

void DoubleActingPneumatic::retract() {
	extendPiston.retract();
	retractPiston.extend();
	extended = false;
}

void DoubleActingPneumatic::toggle() {
	if (extended) retract(); else extend();
}

bool DoubleActingPneumatic::isExtended() const { return extended; }

DoubleActingPneumatic &DoubleActingPneumatic::withHandler(
	pros::controller_digital_e_t btn, std::function<void()> cb)
{
	button = btn;
	callback = cb;
	hasHandler = true;
	return *this;
}