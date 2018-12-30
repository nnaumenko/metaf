# Examples

This directory contains the examples of Metaf parser usage.

[Example: convert METAR or TAF to JSON](https://nnaumenko.gitlab.io/metaf/examples/to_json.html).

## Building

Build examples only:

    emmake make examples

Build entire project:

    emmake make

Pre-built binary files are also included with this repo.

## Description

### Example 1: Parsing METAR or TAF report and generating JSON

This example parses the METAR or TAF report and converts it to JSON.

There are no sanity checks, and malformed report might produce a malformed JSON.

Only minified JSON generation is possible. If "Formatted result" checkbox is set, the formatting is performed using JavaScript by JSON.parse & JSON.stringify sequence.

See file `examples/to_json.cpp` for details.

#### Running

Open file `bin/examples/to_json.html` in www browser (Webassembly support required).

[Or try it on Gitlab Pages](https://nnaumenko.gitlab.io/metaf/examples/to_json.html).

Copy and paste (or manually enter) METAR or TAF report in the "Enter METAR or TAF here" text area, and parsed report in JSON format is displayed in "Conversion result" text area. 

Checking "Formatted result" displays JSON formatted for easier reading. Unchecking "Formatted result" displays minified JSON.

#### Converting METAR report to JSON

Input: 

    UKHH 042230Z 11005MPS 6000 OVC003 M04/M04 Q1022 R07/72//60 NOSIG

Formatted result: `true`

Result: 
<details>
    <summary>Click to show/hide result</summary>
    <pre>
        <code>
{
  "kind": "metaf",
  "report": {
    "type": "METAR",
    "partial": false,
    "icaoLocation": "UKHH",
    "reportReleaseDay": 4,
    "reportReleaseTime": "22:30",
    "wind": {
      "direction": 110,
      "cardinalDirection": "east",
      "speed": 5,
      "speedUnit": "m/s"
    },
    "visibility": [
      {
        "visibility": 6000,
        "visibilityUnit": "meters"
      }
    ],
    "cloudLayers": [
      {
        "amount": "overcast",
        "convectiveType": null,
        "baseHeight": 300,
        "heightUnit": "feet"
      }
    ],
    "airTemperature": -4,
    "airTemperatureFreezing": true,
    "airTemperatureUnit": "centigrade",
    "dewPoint": -4,
    "dewPointUnit": "centigrade",
    "atmosphericPressure": 1022,
    "atmosphericPressureUnit": "hPa",
    "runwayState": [
      {
        "runway": "07",
        "deposits": "ice",
        "contaminationExtent": "11% to 25%",
        "surfaceFriction": {
          "coefficient": 0.6
        }
      }
    ],
    "trends": [
      {
        "trend": "no significant changes"
      }
    ]
  }
}
        </code>
    </pre>
</details>

#### Converting TAF report to JSON

Input: 

    TAF AMD UKLL 041955Z 0419/0518 29006G12MPS 2100 BR BKN003 SCT025CB TX05/0419Z TN00/0518Z
      TEMPO 0419/0424 0700 -SHRA FG BKN002 BKN020CB
      BECMG 0500/0501 3000 BR SCT004 BKN007
      TEMPO 0501/0518 1400 -SHRA BKN003 BKN020CB

Formatted result: `true`

Result:
<details>
    <summary>Click to show/hide result</summary>
    <pre>
        <code>
{
  "kind": "metaf",
  "report": {
    "type": "TAF",
    "partial": false,
    "amendedReport": true,
    "icaoLocation": "UKLL",
    "reportReleaseDay": 4,
    "reportReleaseTime": "19:55",
    "applicableFromDay": 4,
    "applicableFromTime": "19:00",
    "applicableUntilDay": 5,
    "applicableUntilTime": "18:00",
    "wind": {
      "direction": 290,
      "cardinalDirection": "west",
      "speed": 6,
      "gustSpeed": 6,
      "speedUnit": "m/s"
    },
    "visibility": [
      {
        "visibility": 2100,
        "visibilityUnit": "meters"
      }
    ],
    "weather": [
      {
        "weather": "current",
        "proximity": "on site",
        "weatherPhenomena": [
          "mist"
        ]
      }
    ],
    "cloudLayers": [
      {
        "amount": "broken",
        "convectiveType": null,
        "baseHeight": 300,
        "heightUnit": "feet"
      },
      {
        "amount": "scattered",
        "convectiveType": "cumulonimbus",
        "baseHeight": 2500,
        "heightUnit": "feet"
      }
    ],
    "maximumTemperature": {
      "temperatureValue": 5,
      "freezing": false,
      "temperatureUnit": "centigrade",
      "expectedOnDay": 4,
      "expectedOnTime": "19:00"
    },
    "minimumTemperature": {
      "temperatureValue": 0,
      "freezing": false,
      "temperatureDetailed": ">0",
      "temperatureUnit": "centigrade",
      "expectedOnDay": 5,
      "expectedOnTime": "18:00"
    },
    "trends": [
      {
        "trend": "temporary",
        "applicableFromDay": 4,
        "applicableFromTime": "19:00",
        "applicableUntilDay": 4,
        "applicableUntilTime": "24:00",
        "visibility": [
          {
            "visibility": 700,
            "visibilityUnit": "meters"
          }
        ],
        "weather": [
          {
            "weather": "current",
            "proximity": "on site",
            "intensity": "light",
            "weatherPhenomena": [
              "showers",
              "rain"
            ]
          },
          {
            "weather": "current",
            "proximity": "on site",
            "weatherPhenomena": [
              "fog"
            ]
          }
        ],
        "cloudLayers": [
          {
            "amount": "broken",
            "convectiveType": null,
            "baseHeight": 200,
            "heightUnit": "feet"
          },
          {
            "amount": "broken",
            "convectiveType": "cumulonimbus",
            "baseHeight": 2000,
            "heightUnit": "feet"
          }
        ]
      },
      {
        "trend": "temporary",
        "applicableFromDay": 5,
        "applicableFromTime": "00:00",
        "applicableUntilDay": 5,
        "applicableUntilTime": "01:00",
        "visibility": [
          {
            "visibility": 3000,
            "visibilityUnit": "meters"
          }
        ],
        "weather": [
          {
            "weather": "current",
            "proximity": "on site",
            "weatherPhenomena": [
              "mist"
            ]
          }
        ],
        "cloudLayers": [
          {
            "amount": "scattered",
            "convectiveType": null,
            "baseHeight": 400,
            "heightUnit": "feet"
          },
          {
            "amount": "broken",
            "convectiveType": null,
            "baseHeight": 700,
            "heightUnit": "feet"
          }
        ]
      },
      {
        "trend": "temporary",
        "applicableFromDay": 5,
        "applicableFromTime": "01:00",
        "applicableUntilDay": 5,
        "applicableUntilTime": "18:00",
        "visibility": [
          {
            "visibility": 1400,
            "visibilityUnit": "meters"
          }
        ],
        "weather": [
          {
            "weather": "current",
            "proximity": "on site",
            "intensity": "light",
            "weatherPhenomena": [
              "showers",
              "rain"
            ]
          }
        ],
        "cloudLayers": [
          {
            "amount": "broken",
            "convectiveType": null,
            "baseHeight": 300,
            "heightUnit": "feet"
          },
          {
            "amount": "broken",
            "convectiveType": "cumulonimbus",
            "baseHeight": 2000,
            "heightUnit": "feet"
          }
        ]
      }
    ]
  }
}
        </code>
    </pre>
</details>

### Example 2: Explaining METAR or TAF report in plain language

This example parses the METAR or TAF report and generates descriptive text suitable for reading by person not familiar with METAR and TAF format.

There are no sanity checks, and malformed report might produce weird explanation.

See file `examples/explain.cpp` for details.

#### Running

Open file `bin/examples/explain.html` in www browser (Webassembly support required).

[Or try it on Gitlab Pages](https://nnaumenko.gitlab.io/metaf/examples/explain.html).

Copy and paste (or manually enter) METAR or TAF report in the "Enter METAR or TAF here" text area, and decoded report in plain language is displayed under Report Explanation.

For example entering the following METAR report: 

    NZSP 042250Z 08008KT 9999 FEW005 FEW030 M29/ A2869 RMK FG DSNT GRID S CLN AIR 09009KT ALL WNDS GRID SDG/HDG

produces the following plain language explanation:

<details>
    <summary>Click to show/hide result</summary>
    <pre>
Detected report type: METAR (weather observation)
NZSP : ICAO code for location: NZSP
042250Z : Day and time of report issue: day 4, time 22:50 GMT
08008KT : Wind direction: 80° (east) Wind speed:8 knots (4.1 m/s, 14.8 km/h, 9.2 mph)
9999 : Visibility (prevailing) >10000 meters (>6.21 statute miles, >32808 feet)|
FEW005 : Few clouds (1/8 to 2/8 sky covered), base height 500 feet (152 meters, 0.09 statute miles)
FEW030 : Few clouds (1/8 to 2/8 sky covered), base height 3000 feet (914 meters, 0.56 statute miles)
M29/ : Air temperature: -29 °C (-20 °F), dew point: not reported
A2869 : Atmospheric pressure: 28.69 inHg (971hPa)
RMK : The remarks are as follows (this version does not recognise or decode remarks)
FG : This group is not recognised by parser: FG
DSNT : This group is not recognised by parser: DSNT
GRID : This group is not recognised by parser: GRID
S : This group is not recognised by parser: S
CLN : This group is not recognised by parser: CLN
AIR : This group is not recognised by parser: AIR
09009KT : This group is not recognised by parser: 09009KT
ALL : This group is not recognised by parser: ALL
WNDS : This group is not recognised by parser: WNDS
GRID : This group is not recognised by parser: GRID
SDG/HDG : This group is not recognised by parser: SDG/HDG
    </pre>
</details>