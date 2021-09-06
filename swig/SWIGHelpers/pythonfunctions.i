///////////////////////////////////////////////
//              Python stuff
///////////////////////////////////////////////
%pythoncode %{


def now(timeSystem=TimeSystem.UTC):
    """
    Returns the current time (defined by what SystemTime() returns)
    in a CommonTime format, in the given TimeSystem.

    Parameters:
            -----------

        timeSystem:  the TimeSystem to assign to the output
    """
    t = SystemTime().toCommonTime()
    t.setTimeSystem(timeSystem)
    return t


def times(starttime, endtime, seconds=0.0, days=0):
    """
    Returns a generator expression of CommonTime objects between (or equal to)
    starttime and endtime.

    You may specify a timestep in seconds (floating/integral type)
    and/or days (integral type). Not specifying a timestep will
    return a generator that yields the starttime and endtime parameters.
    The timestep must be positive, or a gpstk.exceptions.InvalidRequest
    will be raised.
    """
    if (seconds < 0.0) or (days < 0):
        raise InvalidRequest('Negative time steps may not be used.')
        return

    if (seconds == 0.0) and (days == 0):
        # empty generator:
        yield starttime
        yield endtime
        return

    t = CommonTime(starttime)
    while t <= endtime:
        yield CommonTime(t)
        t.addSeconds(seconds)
        t.addDays(days)


def klobuchar_correction(iono, com_time, rx_pos, elev, azim, band=CarrierBand.L1):
    """
    Returns the Klobuchar model ionospheric correction for a time, position, SV elev/azim, and GPS band (L1,L2,L5).
    """
    return iono.getCorrection(com_time, rx_pos, float(elev), float(azim), band)


def moonPosition(time):
    """
    Returns the current position (A gpstk.Triple) of the moon.
    This is a functional wrapper on the moonPosition class.
    """
    return MoonPosition().getPosition(time)


def sunPosition(time):
    """
    Returns the current position (A gpstk.Triple) of the moon.
    This is a functional wrapper on the MoonPosition class.
    """
    return SunPosition().getPosition(time)


def poleTides(time, position, x, y):
    """
    Returns the effect (a gpstk.Triple) of pole tides (meters)
    on the given position, in the Up-East-North (UEN) reference frame.
    This is a functional wrapper on the (hidden) PoleTides class.
    """
    return PoleTides().getPoleTide(time, position, x, y)


def solidTides(time, position):
    """
    Returns the effect (a gpstk.Triple) of solid Earth tides (meters)
    at the given position and epoch, in the Up-East-North (UEN) reference frame.
    This is a functional wrapper on the (hidden) SolidTides class.
    """
    return SolidTides().getSolidTide(time, position)


def cartesian(x=0.0, y=0.0, z=0.0,
              model=None, frame=ReferenceFrame.Unknown):
    "Returns a Position in the Cartesian coordinate system."
    return Position(x, y, z, Position.Cartesian, model, frame)


def geodetic(latitude=0.0, longitude=0.0, height=0.0,
             model=None, frame=ReferenceFrame.Unknown):
    "Returns a Position in the Geodetic coordinate system."
    return Position(latitude, longitude, height, Position.Geodetic, model, frame)


def spherical(theta=0.0, phi=0.0, radius=0.0,
              model=None, frame=ReferenceFrame.Unknown):
    "Returns a Position in the Spherical coordinate system."
    return Position(theta, phi, radius, Position.Spherical, model, frame)


def geocentric(latitude=0.0, longitude=0.0, radius=0.0,
               model=None, frame=ReferenceFrame.Unknown):
    "Returns a Position in the Geocentric coordinate system."
    return Position(latitude, longitude, radius, Position.Geocentric, model, frame)


def eval(expr, **kwargs):
    e = Expression(expr)
    e.setGPSConstants()
    for key in kwargs:
        e.set(key, kwargs[key])
    return e.evaluate()


%}
