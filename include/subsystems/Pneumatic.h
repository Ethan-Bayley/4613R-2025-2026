#pragma once

#include "subsystems/Subsystems.h"
#include "pros/adi.hpp"
#include <functional>

// ─── Single Acting ────────────────────────────────────────────────────────────
class SingleActingPneumatic : public Subsystems
{
public:
	SingleActingPneumatic(int pistonPort, bool startExtended = false);

	void setup() override;
	void registerHandlers(EventLoop &eventLoop) override;

	void extend();
	void retract();
	void toggle();
	bool isExtended() const;

	// Call before adding to robot to bind a button
	SingleActingPneumatic &withHandler(pros::controller_digital_e_t button,
									   std::function<void()> callback = nullptr);
	SingleActingPneumatic &withName(const std::string &n)
	{
		name = n;
		return *this;
	}
	const std::string &getName() const { return name; }

private:
	std::string name;
	pros::adi::Pneumatics piston;
	bool extended;
	pros::controller_digital_e_t button;
	std::function<void()> callback;
	bool hasHandler = false;
};

// ─── Double Acting ────────────────────────────────────────────────────────────
class DoubleActingPneumatic : public Subsystems
{
public:
	DoubleActingPneumatic(int extendPort, int retractPort, bool startExtended = false);

	void setup() override;
	void registerHandlers(EventLoop &eventLoop) override;

	void extend();
	void retract();
	void toggle();
	bool isExtended() const;

	// Call before adding to robot to bind a button
	DoubleActingPneumatic &withHandler(pros::controller_digital_e_t button,
									   std::function<void()> callback = nullptr);
	DoubleActingPneumatic &withName(const std::string &n)
	{
		name = n;
		return *this;
	}

private:
	std::string name;
	pros::adi::Pneumatics extendPiston;
	pros::adi::Pneumatics retractPiston;
	bool extended;
	pros::controller_digital_e_t button;
	std::function<void()> callback;
	bool hasHandler = false;
};