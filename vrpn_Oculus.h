/** @file	vrpn_Oculus.h
	@brief	Header for various Oculus devices.  Initially, just the DK2.

	@date	2015
  @copyright 2015 ReliaSolve.com
  @author ReliaSolve.com russ@reliasolve.com
  @license Standard VRPN license.
*/

// Based on the OSVR hacker dev kit driver by Kevin Godby.
// Based on Oliver Kreylos' OculusRiftHIDReports.cpp.

#pragma once

#include "vrpn_HumanInterface.h"
#include "vrpn_Analog.h"

#if defined(VRPN_USE_HID)

/// @brief Oculus Rift DK2 head-mounted display base class for both intertial-
/// only and LED-enabled version.
class VRPN_API vrpn_Oculus_DK2 : public vrpn_Analog, protected vrpn_HidInterface {
public:
    /**
     * @brief Destructor.
     */
    virtual ~vrpn_Oculus_DK2();

    virtual void mainloop();

protected:
  /**
  * @brief Protected constructor so you can't instantiate this base class.
  *
  * @param name Name of this device.
  * @param c Optional vrpn_Connection pointer to use as our connection.
  * @param keepAliveSeconds How often to re-trigger the LED flashing
  */
  vrpn_Oculus_DK2(bool enableLEDs, const char *name, vrpn_Connection *c = NULL,
    double keepAliveSeconds = 9.0);

  vrpn_HidAcceptor *m_filter;

  //-------------------------------------------------------------
  // Parsers for different report types.  The DK2 sends type-1
  // reports in response to inertial-only keep-alive messages
  // and type-11 reports in response to LED-enabled keep-alive
  // messages.

  /// Parse and send reports for type-1 message
  void parse_message_type_1(std::size_t bytes, vrpn_uint8 *buffer);

  /// Parse and send reports for type-11 message
  void parse_message_type_11(std::size_t bytes, vrpn_uint8 *buffer);

  /// Extracts the sensor values from each report and calls the
  /// appropriate parser.
  void on_data_received(std::size_t bytes, vrpn_uint8 *buffer);

  /// Timestamp updated during mainloop()
  struct timeval d_timestamp;

  /// How often to send the keepAlive message to trigger the LEDs
  bool d_enableLEDs;
  double d_keepAliveSeconds;
  struct timeval d_lastKeepAlive;

  // Send a KeepAlive feature report to the DK2.  This needs to be sent
  // every keepAliveSeconds to keep the LEDs going.
  void writeKeepAlive(
    vrpn_uint16 interval = 10000 //< KeepAlive time in milliseconds
    , vrpn_uint16 commandId = 0 //< Should always be zero
    );
};

/// @brief Oculus Rift DK2 head-mounted display (inertial measurement unit only)
class VRPN_API vrpn_Oculus_DK2_inertial : public vrpn_Oculus_DK2 {
public:
  /**
  * @brief Constructor
  *
  * @param name Name of this device.
  * @param c Optional vrpn_Connection pointer to use as our connection.
  * @param keepAliveSeconds How often to re-trigger the LED flashing
  */
  vrpn_Oculus_DK2_inertial(const char *name, vrpn_Connection *c = NULL,
    double keepAliveSeconds = 9.0)
    : vrpn_Oculus_DK2(false, name, c, keepAliveSeconds) {};
};

/// @brief Oculus Rift DK2 head-mounted display (LEDs enabled)
class VRPN_API vrpn_Oculus_DK2_LEDs : public vrpn_Oculus_DK2 {
public:
  /**
  * @brief Constructor
  *
  * @param name Name of this device.
  * @param c Optional vrpn_Connection pointer to use as our connection.
  * @param keepAliveSeconds How often to re-trigger the LED flashing
  */
  vrpn_Oculus_DK2_LEDs(const char *name, vrpn_Connection *c = NULL,
    double keepAliveSeconds = 9.0)
    : vrpn_Oculus_DK2(true, name, c, keepAliveSeconds) {};
};

#endif // VRPN_USE_HID
