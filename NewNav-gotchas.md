\page NewNavGotchas NewNav Gotchas

Here are some important things to be aware of if you are in the
process of migrating from the legacy navigation data classes (OrbElem,
GPSEphemeris, etc.) to NewNav.

* NavLibrary::getInitialTime() and NavLibrary::getFinalTime() (and
  their equivalents in the NavDataFactory class tree) behave somewhat
  differently from XvtStore.  This is largely due to the next item.
* The NewNav OrbitDataKepler classes may produce different validity
  times (beginFit and endFit) from the legacy classes.  In these
  cases, the validity times were either updated to match the
  appropriate ICD, or if such information was not available in an ICD,
  a fit interval was acquired from analysis \cite johnson.
* When searching NavLibrary or a NavDataFactory class for navigation
  data, the "User" type search behaves differently in that it now
  takes into account the time it takes for a satellite to transmit a
  message, e.g. a user wanting an GPS LNAV ephemeris at 00:00 isn't
  going to have the ephemeris transmited starting at 00:00, they'd
  have to wait until the complete ephemeris was received 18 seconds
  plus propagation time later.  The code does not take the propagation
  time into account when searching, as that is geometry-dependent, but
  it does take into account whether the complete message had been
  transmitted by the satellite.  There are two ways to deal with this,
  and it will depend on your use case as to which is better for you.
  One is to simply switch to using "Nearest" mode searches.  If you
  really need "User" mode searches for your use case, and need data at
  the leading edge, you will need to load more data to provide
  matching navigation data.
    
