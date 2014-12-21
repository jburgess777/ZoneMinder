#ifndef ZM_DETECTOR_H
#define ZM_DETECTOR_H


#include <string>
#include <algorithm>
#include <syslog.h>
#include <libgen.h>

#include "zm_image.h"
#include "zm_zone.h"
#include "zm_event.h"

#define DEFAULT_DETECTION_CAUSE "Object Detected"
#define DEFAULT_MIN_ALARM_SCORE 1.0
#define DEFAULT_MAX_ALARM_SCORE 99.0
#define DEFAULT_IMAGE_SCALE_FACTOR 1.0

#define DEFAULT_LOG_PREFIX "ZM PLUGIN"
#define LOG_LEVEL LOG_NOTICE
#define DEFAULT_CONFIGFILE_SECTION "libzm_vscvl_plugin"

using namespace std;


//! Base class for object detectors, defined in plugins.
class Detector
{

public:

    //! Destructor
    virtual ~Detector() {}

    //! Default constructor
    Detector()
{
    m_sLogPrefix = DEFAULT_LOG_PREFIX;
    m_sDetectionCause = DEFAULT_DETECTION_CAUSE;
    m_fMinAlarmScore = DEFAULT_MIN_ALARM_SCORE;
    m_fMaxAlarmScore = DEFAULT_MAX_ALARM_SCORE;
    m_fImageScaleFactor = DEFAULT_IMAGE_SCALE_FACTOR;
    m_sConfigSectionName = DEFAULT_CONFIGFILE_SECTION;
    m_nNewWidth = 0;
    m_nNewHeight = 0;
}

    //! Constructor with section name parameter.
    Detector(string sPluginFileName)
{
    m_sLogPrefix = DEFAULT_LOG_PREFIX;

    char* szPluginFileName = strdup(sPluginFileName.c_str());

    string sPluginFileNameName = string(basename(szPluginFileName));

    size_t idx = sPluginFileNameName.rfind('.');

    if (idx == string::npos)
        m_sConfigSectionName = sPluginFileNameName;
    else
        m_sConfigSectionName = sPluginFileNameName.substr(0, idx);

    m_sDetectionCause = DEFAULT_DETECTION_CAUSE;
    m_fMinAlarmScore = DEFAULT_MIN_ALARM_SCORE;
    m_fMaxAlarmScore = DEFAULT_MAX_ALARM_SCORE;
    m_fImageScaleFactor = DEFAULT_IMAGE_SCALE_FACTOR;
    m_nNewWidth = 0;
    m_nNewHeight = 0;
}

    //! Copy constructor
    Detector(const Detector& source);

    //! Assignment operator
    Detector& operator=(const Detector& source);

    //! Detect (in an image later)
    bool Detect(const Image* image, Zone** zones, Event::StringSet &zoneSet, unsigned int &score);

    //! Load detector's parameters.
    virtual int loadConfig(string sConfigFileName, map<unsigned int,map<string,string> > mapPluginConf) = 0;

    //! Returns detection case string.
    string getDetectionCause();

    //! Returns plugin name as string.
    string getPluginName();

    //! Enable the plugin for the given zones.
    void EnablePlugin(vector<unsigned int> zoneList);

    //! Return the list of enabled zones
    vector<unsigned int> getPluginZones();

protected:

    //! Do detection inside one given zone.
    virtual bool checkZone(Zone *zone, unsigned int n_zone, const Image *zmImage) = 0;

    //! Log messages to the SYSLOG.
    void log(int, string sLevel, string sMessage);

    //! String to be shown as detection cause for event.
    string m_sDetectionCause;

    //! Minimum score value to consider frame as to be alarmed.
    double m_fMinAlarmScore;

    //! Maximum score value to consider frame as to be alarmed.
    double m_fMaxAlarmScore;

    //! Maximum allowed width of frame image.
    double m_fImageScaleFactor;

    //! Width of image to resize.
    int m_nNewWidth;

    //! Height of image to resize.
    int m_nNewHeight;

    //! String prefix for SYSLOG messages.
    string m_sLogPrefix;

    //! Name of config file section to search parameters.
    string m_sConfigSectionName;

    //! List of zones enabled for the plugin
    vector<unsigned int> m_vnPluginZones;

    //! Plugin status regarding zone settings
    bool m_bIsPluginEnabled;

};


#endif // ZM_DETECTOR_H
