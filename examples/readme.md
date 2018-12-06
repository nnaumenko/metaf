# Examples

## Compiling

Examples are compiled with command `emmake make examples`.

## Example 1: Parsing METAR or TAF report and generating JSON

### Description

This example parses the METAR or TAF report and converts it to JSON.

There are no sanity checks, and malformed report might produce a malformed JSON.

Only minified JSON generation is possible. If "Formatted result" checkbox is set, the formatting is performed using JavaScript by JSON.parse & JSON.stringify sequence.

See file `metar_taf_to_json.cpp` for details.

### Running

[Run this example at Gitlab Pages](https://nnaumenko.gitlab.io/metaf/examples/to_json.html).

Example can also be run locally by openening file `bin/examples/to_json.html` in browser.

Copy and paste (or manually enter) METAR or TAF report in the "Enter METAR or TAF here" text area, and parsed report in JSON format is displayed in "Conversion result" text area. 

Checking "Formatted result" displays JSON formatted for easier reading. Unchecking "Formatted result" displays minified JSON.

The following sample data generally demonstrate conversion principle.

#### Sample data 1

Input: 

    UKHH 042230Z 11005MPS 6000 OVC003 M04/M04 Q1022 R07/72//60 NOSIG

Formatted result: `true`

Result: 

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
        "dewPoint": -4,
        "temperatureUnit": "centigrade",
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
    

#### Sample data 2

Input: 

    TAF AMD UKLL 041955Z 0419/0518 29006G12MPS 2100 BR BKN003 SCT025CB TX05/0419Z TN00/0518Z
      TEMPO 0419/0424 0700 -SHRA FG BKN002 BKN020CB
      BECMG 0500/0501 3000 BR SCT004 BKN007
      TEMPO 0501/0518 1400 -SHRA BKN003 BKN020CB

Formatted result: `true`

Result:

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
          "temperatureUnit": "centigrade",
          "expectedOnDay": 4,
          "expectedOnTime": "19:00"
        },
        "minimumTemperature": {
          "temperatureValue": 0,
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
    
