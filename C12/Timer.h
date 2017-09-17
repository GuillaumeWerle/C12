// [12/5/2010 Guillaume] Created

#pragma once


	// Timer class.
	// Call Tick() in the mainloop.
class Timer
{
public:
	void	SetTimeSinceStart(double t) { m_timeSinceStart = t; }
	double	GetTimeSinceStart() const { return m_timeSinceStart; }
	float	GetDelta() const { return m_delta; }
	double	GetDeltaPrecise() const { return m_deltad; }

	// Call this once per update 
	double Tick();
	void Restart();

	Timer();
	virtual ~Timer();

private:
	LONGLONG m_last;
	double m_timeSinceStart;
	float	m_delta;
	double	m_deltad;
	double m_invFrequency;
};

// GameTimer class.
// Same as Timer but with 'Paused' state.
// Note : if paused then the delta == 0
class  GameTimer
{
public:

	// Speed factor, set to 0 for pause.
	float	GetSpeed() const { return m_speed; }
	void	SetSpeed(float val) { m_speed = val; }

	// true if speed == 0
	bool	Paused() const { return m_speed == 0; }

	// Time in second since Restart was called.
	double	GetTimeSinceStart() const { return m_timeSinceStart; }

	// Time in second since Tick was called.
	double	GetDelta() const { return m_delta; }

	// To be called each frame
	void	Tick();

	// To be called at startup
	void	Restart();

	GameTimer();

protected:
private:

	double m_timeSinceStart;
	double m_delta;
	Timer m_timer;
	float m_speed;

};


// Delta time average to smooth out possible timing hiccups.
// Pass the new delta to NotifyNewDelta() and get the results using ComputeAverage().
class  TimeAverage
{

public:
	void NotifyNewDelta(float d);
	float ComputeAverage() const;
	TimeAverage();

private:
	float m_times[8];
	unsigned int m_index;
};

