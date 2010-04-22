// ***************************************************************** -*- C++ -*-
/*
 * Copyright (C) 2004-2010 Andreas Huggel <ahuggel@gmx.net>
 *
 * This program is part of the Exiv2 distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, 5th Floor, Boston, MA 02110-1301 USA.
 */
/*
  File:      sonymn.cpp
  Version:   $Rev$
  Author(s): Andreas Huggel (ahu) <ahuggel@gmx.net>
  History:   18-Apr-05, ahu: created
 */
// *****************************************************************************
#include "rcsid.hpp"
EXIV2_RCSID("@(#) $Id$")

// *****************************************************************************
// included header files
#include "types.hpp"
#include "minoltasonyvalues.hpp"
#include "sonymn.hpp"
#include "value.hpp"
#include "i18n.h"                // NLS support.

// + standard includes
#include <string>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <cstring>

// *****************************************************************************
// class member definitions
namespace Exiv2 {

    // -- Standard Sony Makernotes tags ---------------------------------------------------------------

    //! Lookup table to translate Sony image quality values to readable labels
    extern const TagDetails sonyImageQuality[] = {
        { 0, N_("Raw")                   },
        { 1, N_("Super Fine")            },
        { 2, N_("Fine")                  },
        { 3, N_("Standard")              },
        { 4, N_("Economy")               },
        { 5, N_("Extra Fine")            },
        { 6, N_("Raw + JPEG")            },
        { 7, N_("Compressed Raw")        },
        { 8, N_("Compressed Raw + JPEG") }
    };

    //! Lookup table to translate Sony teleconverter model values to readable labels
    extern const TagDetails sonyTeleconverterModel[] = {
        { 0x00, N_("None")                     },
        { 0x48, N_("Minolta AF 2x APO (D)")    },
        { 0x50, N_("Minolta AF 2x APO II")     },
        { 0x88, N_("Minolta AF 1.4x APO (D)")  },
        { 0x90, N_("Minolta AF 1.4x APO II")   }
    };

    //! Lookup table to translate Sony Std camera settings white balance values to readable labels
    extern const TagDetails sonyWhiteBalanceStd[] = {
        { 0x00,  N_("Auto")                           },
        { 0x01,  N_("Color Temperature/Color Filter") },
        { 0x10,  N_("Daylight")                       },
        { 0x20,  N_("Cloudy")                         },
        { 0x30,  N_("Shade")                          },
        { 0x40,  N_("Tungsten")                       },
        { 0x50,  N_("Flash")                          },
        { 0x60,  N_("Fluorescent")                    },
        { 0x70,  N_("Custom")                         }
    };

    //! Lookup table to translate Sony Auto HDR values to readable labels
    extern const TagDetails sonyAutoHDR[] = {
        { 0x00000, N_("Off") },
        { 0x10001, N_("On")  }
    };

    //! Lookup table to translate Sony model ID values to readable labels
    extern const TagDetails sonyModelId[] = {
        { 2,   "DSC-R1"    },
        { 256, "DSLR-A100" },
        { 257, "DSLR-A900" },
        { 258, "DSLR-A700" },
        { 259, "DSLR-A200" },
        { 260, "DSLR-A350" },
        { 261, "DSLR-A300" },
        { 263, "DSLR-A380" },
        { 264, "DSLR-A330" },
        { 265, "DSLR-A230" },
        { 269, "DSLR-A850" },
        { 273, "DSLR-A550" },
        { 274, "DSLR-A500" },
        { 275, "DSLR-A450" }
    };

    //! Lookup table to translate Sony scene mode values to readable labels
    extern const TagDetails sonySceneMode[] = {
        { 0,  N_("Standard")            },
        { 1,  N_("Portrait")            },
        { 2,  N_("Text")                },
        { 3,  N_("Night Scene")         },
        { 4,  N_("Sunset")              },
        { 5,  N_("Sports")              },
        { 6,  N_("Landscape")           },
        { 7,  N_("Night Portrait")      },
        { 8,  N_("Macro")               },
        { 9,  N_("Super Macro")         },
        { 16, N_("Auto")                },
        { 17, N_("Night View/Portrait") }
    };

    //! Lookup table to translate Sony zone matching values to readable labels
    extern const TagDetails sonyZoneMatching[] = {
        { 0, N_("ISO Setting Used") },
        { 1, N_("High Key") },
        { 2, N_("Low Key")  }
    };

    //! Lookup table to translate Sony dynamic range optimizer values to readable labels
    extern const TagDetails print0xb025[] = {
        { 0,  N_("Off")           },
        { 1,  N_("Standard ")     },
        { 2,  N_("Advanced Auto") },
        { 3,  N_("Auto")          },
        { 8,  N_("Advanced Lv1")  },
        { 9,  N_("Advanced Lv2")  },
        { 10, N_("Advanced Lv3")  },
        { 11, N_("Advanced Lv4")  },
        { 12, N_("Advanced Lv5")  }
    };

    //! Lookup table to translate Sony exposure mode values to readable labels
    extern const TagDetails sonyExposureMode[] = {
        { 0,  N_("Auto")                     },
        { 1,  N_("Portrait")                 },
        { 2,  N_("Beach")                    },
        { 4,  N_("Snow")                     },
        { 5,  N_("Landscape ")               },
        { 6,  N_("Program")                  },
        { 7,  N_("Aperture priority")        },
        { 8,  N_("Shutter priority")         },
        { 9,  N_("Night Scene / Twilight")   },
        { 10, N_("Hi-Speed Shutter")         },
        { 11, N_("Twilight Portrait")        },
        { 12, N_("Soft Snap")                },
        { 13, N_("Fireworks")                },
        { 15, N_("Manual")                   },
        { 18, N_("High Sensitivity")         },
        { 20, N_("Advanced Sports Shooting") },
        { 29, N_("Underwater")               },
        { 33, N_("Gourmet")                  },
        { 34, N_("Panorama")                 },
        { 35, N_("Handheld Twilight")        },
        { 36, N_("Anti Motion Blur")         },
        { 37, N_("Pet")                      },
        { 38, N_("Backlight Correction HDR") }
    };

    //! Lookup table to translate Sony Quality values to readable labels
    extern const TagDetails sonyQuality[] = {
        { 0, N_("Normal") },
        { 1, N_("Fine")   }
    };

    //! Lookup table to translate Sony anti-blur values to readable labels
    extern const TagDetails sonyAntiBlur[] = {
        { 0,     N_("Off")             },
        { 1,     N_("On (Continuous)") },
        { 2,     N_("On (Shooting)")   },
        { 65535, N_("Not Applicable")  }
    };

    //! Lookup table to translate Sony dynamic range optimizer values to readable labels
    extern const TagDetails print0xb04f[] = {
        { 0, N_("Off")      },
        { 1, N_("Standard") },
        { 2, N_("Plus")     }
    };

    //! Lookup table to translate Sony Intelligent Auto values to readable labels
    extern const TagDetails sonyIntelligentAuto[] = {
        { 0, N_("Off")      },
        { 1, N_("On")       },
        { 2, N_("Advanced") }
    };

    //! Lookup table to translate Sony WB values to readable labels
    extern const TagDetails sonyWhiteBalance[] = {
        { 0,  N_("Auto")          },
        { 4,  N_("Manual")        },
        { 5,  N_("Daylight")      },
        { 14, N_("Incandescent")  }
    };

    std::ostream& SonyMakerNote::print0xb000(std::ostream& os, const Value& value, const ExifData*)
    {
        if (value.count() != 4)
        {
            os << "(" << value << ")";
        }
        else
        {
            std::string val = value.toString(0) + value.toString(1) + value.toString(2) + value.toString(3);
            if      (val == "0002") os << "JPEG";
            else if (val == "1000") os << "SR2";
            else if (val == "2000") os << "ARW 1.0";
            else if (val == "3000") os << "ARW 2.0";
            else if (val == "3100") os << "ARW 2.1";
            else                    os << "(" << value << ")";
        }
        return os;
    }

    std::ostream& SonyMakerNote::printImageSize(std::ostream& os, const Value& value, const ExifData*)
    {
        if (value.count() == 2)
            os << value.toString(0) << " x " << value.toString(1);
        else
            os << "(" << value << ")";

        return os;
    }

    // Sony MakerNote Tag Info
    const TagInfo SonyMakerNote::tagInfo_[] = {
        TagInfo(0x0102, "Quality", N_("Image Quality"),
                N_("Image quality"),
                sony1IfdId, makerTags, unsignedLong, EXV_PRINT_TAG(sonyImageQuality)),
        TagInfo(0x0104, "FlashExposureComp", N_("Flash Exposure Compensation"),
                N_("Flash exposure compensation in EV"),
                sony1IfdId, makerTags, signedRational, printValue),
        TagInfo(0x0105, "Teleconverter", N_("Teleconverter Model"),
                N_("Teleconverter Model"),
                sony1IfdId, makerTags, unsignedLong, EXV_PRINT_TAG(sonyTeleconverterModel)),
        TagInfo(0x0112, "WhiteBalanceFineTune", N_("White Balance Fine Tune"),
                N_("White Balance Fine Tune Value"),
                sony1IfdId, makerTags, unsignedLong, printValue),
        TagInfo(0x0114, "CameraSettings", N_("Camera Settings"),
                N_("Camera Settings"),
                sony1IfdId, makerTags, undefined, printValue),
        TagInfo(0x0115, "WhiteBalance", N_("White Balance"),
                N_("White balance"),
                sony1IfdId, makerTags, unsignedLong, EXV_PRINT_TAG(sonyWhiteBalanceStd)),
        TagInfo(0x0116, "0x0116", "0x0116",
                N_("Unknown"),
                sony1IfdId, makerTags, undefined, printValue),
        TagInfo(0x0e00, "PrintIM", N_("Print IM"),
                N_("PrintIM information"),
                sony1IfdId, makerTags, undefined, printValue),
        TagInfo(0x2000, "0x2000", "0x2000",
                N_("Unknown"),
                sony1IfdId, makerTags, undefined, printValue),
        TagInfo(0x2001, "PreviewImage", N_("Preview Image"),
                N_("Preview Image"),
                sony1IfdId, makerTags, undefined, printValue),
        TagInfo(0x2002, "0x2002", "0x2002",
                N_("Unknown"),
                sony1IfdId, makerTags, unsignedLong, printValue),
        TagInfo(0x2003, "0x2003", "0x2003",
                N_("Unknown"),
                sony1IfdId, makerTags, asciiString, printValue),
        TagInfo(0x2004, "0x2004", "0x2004",
                N_("Unknown"),
                sony1IfdId, makerTags, signedLong, printValue),
        TagInfo(0x2005, "0x2005", "0x2005",
                N_("Unknown"),
                sony1IfdId, makerTags, signedLong, printValue),
        TagInfo(0x2006, "0x2006", "0x2006",
                N_("Unknown"),
                sony1IfdId, makerTags, signedLong, printValue),
        TagInfo(0x2007, "0x2007", "0x2007",
                N_("Unknown"),
                sony1IfdId, makerTags, signedLong, printValue),
        TagInfo(0x2008, "0x2008", "0x2008",
                N_("Unknown"),
                sony1IfdId, makerTags, signedLong, printValue),
        TagInfo(0x2009, "0x2009", "0x2009",
                N_("Unknown"),
                sony1IfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x200A, "AutoHDR", N_("Auto HDR"),
                N_("Auto High Definition Range"),
                sony1IfdId, makerTags, unsignedLong, EXV_PRINT_TAG(sonyAutoHDR)),
        TagInfo(0x3000, "ShotInfo", N_("Shot Info"),
                N_("Shot Information"),
                sony1IfdId, makerTags, undefined, printValue),
        TagInfo(0xb000, "FileFormat", N_("File Format"),
                N_("File Format"),
                sony1IfdId, makerTags, unsignedByte, print0xb000),
        TagInfo(0xb001, "SonyModelID", N_("Sony Model ID"),
                N_("Sony Model ID"),
                sony1IfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyModelId)),
        TagInfo(0xb020, "ColorReproduction", N_("Color Reproduction"),
                N_("Color Reproduction"),
                sony1IfdId, makerTags, asciiString, printValue),
        TagInfo(0xb021, "ColorTemperature", N_("Color Temperature"),
                N_("Color Temperature"),
                sony1IfdId, makerTags, unsignedLong, printValue),
        TagInfo(0xb022, "ColorCompensationFilter", N_("Color Compensation Filter"),
                N_("Color Compensation Filter: negative is green, positive is magenta"),
                sony1IfdId, makerTags, unsignedLong, printValue),
        TagInfo(0xb023, "SceneMode", N_("Scene Mode"),
                N_("Scene Mode"),
                sony1IfdId, makerTags, unsignedLong, EXV_PRINT_TAG(sonySceneMode)),
        TagInfo(0xb024, "ZoneMatching", N_("Zone Matching"),
                N_("Zone Matching"),
                sony1IfdId, makerTags, unsignedLong, EXV_PRINT_TAG(sonyZoneMatching)),
        TagInfo(0xb025, "DynamicRangeOptimizer", N_("Dynamic Range Optimizer"),
                N_("Dynamic Range Optimizer"),
                sony1IfdId, makerTags, unsignedLong, EXV_PRINT_TAG(print0xb025)),
        TagInfo(0xb026, "ImageStabilization", N_("Image Stabilization"),
                N_("Image stabilization"),
                sony1IfdId, makerTags, unsignedLong, printMinoltaSonyBoolValue),
        TagInfo(0xb027, "LensID", N_("Lens ID"),
                N_("Lens identifier"),
                sony1IfdId, makerTags, unsignedLong, printMinoltaSonyLensID),
        TagInfo(0xb028, "MinoltaMakerNote", N_("Minolta MakerNote"),
                N_("Minolta MakerNote"),
                sony1IfdId, makerTags, undefined, printValue),
        TagInfo(0xb029, "ColorMode", N_("Color Mode"),
                N_("Color Mode"),
                sony1IfdId, makerTags, unsignedLong, printMinoltaSonyBoolValue),
        TagInfo(0xb02b, "FullImageSize", N_("Full Image Size"),
                N_("Full Image Size"),
                sony1IfdId, makerTags, unsignedLong, printImageSize),
        TagInfo(0xb02c, "PreviewImageSize", N_("Preview Image Size"),
                N_("Preview Image Size"),
                sony1IfdId, makerTags, unsignedLong, printImageSize),
        TagInfo(0xb040, "Macro", N_("Macro"),
                N_("Macro"),
                sony1IfdId, makerTags, unsignedShort, printMinoltaSonyBoolValue),
        TagInfo(0xb041, "ExposureMode", N_("Exposure Mode"),
                N_("Exposure Mode"),
                sony1IfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyExposureMode)),
        TagInfo(0xb047, "Quality", N_("Quality"),
                N_("Quality"),
                sony1IfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyQuality)),
        TagInfo(0xb04b, "AntiBlur", N_("Anti-Blur"),
                N_("Anti-Blur"),
                sony1IfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyAntiBlur)),
        TagInfo(0xb04e, "LongExposureNoiseReduction", N_("Long Exposure Noise Reduction"),
                N_("Long Exposure Noise Reduction"),
                sony1IfdId, makerTags, unsignedShort, printMinoltaSonyBoolValue),
        TagInfo(0xb04f, "DynamicRangeOptimizer", N_("Dynamic Range Optimizer"),
                N_("Dynamic Range Optimizer"),
                sony1IfdId, makerTags, unsignedShort, EXV_PRINT_TAG(print0xb04f)),
        TagInfo(0xb052, "IntelligentAuto", N_("Intelligent Auto"),
                N_("Intelligent Auto"),
                sony1IfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyIntelligentAuto)),
        TagInfo(0xb054, "WhiteBalance", N_("White Balance"),
                N_("White Balance"),
                sony1IfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyWhiteBalance)),

        // End of list marker
        TagInfo(0xffff, "(UnknownSony1MakerNoteTag)", "(UnknownSony1MakerNoteTag)",
                N_("Unknown Sony1MakerNote tag"),
                sony1IfdId, makerTags, invalidTypeId, printValue)
    };

    const TagInfo* SonyMakerNote::tagList()
    {
        return tagInfo_;
    }

    // -- Sony camera settings ---------------------------------------------------------------

    //! Lookup table to translate Sony camera settings drive mode values to readable labels
    extern const TagDetails sonyDriveModeStd[] = {
        { 1, N_("Single Frame")                      },
        { 2, N_("Continuous High")                   },
        { 4, N_("Self-timer 10 sec")                 },
        { 5, N_("Self-timer 2 sec")                  },
        { 7, N_("Continuous Bracketing")             },
        { 12, N_("Continuous Low")                   },
        { 18, N_("White Balance Bracketing Low")     },
        { 19, N_("D-Range Optimizer Bracketing Low") }
    };

    //! Lookup table to translate Sony camera settings rotation values to readable labels
    extern const TagDetails sonyRotation[] = {
        { 0, N_("Horizontal (normal)") },
        { 1, N_("Rotate 90 CW")        },
        { 2, N_("Rotate 270 CW")       }
    };

    //! Lookup table to translate Sony camera settings focus mode values to readable labels
    extern const TagDetails sonyFocusMode[] = {
        { 0, N_("Manual") },
        { 1, "AF-S"       },
        { 2, "AF-C"       },
        { 3, "AF-A"       }
    };

    //! Lookup table to translate Sony camera settings metering mode values to readable labels
    extern const TagDetails sonyMeteringMode[] = {
        { 1, N_("Multi-segment")   },
        { 2, N_("Center weighted") },
        { 4, N_("Spot")            }
    };

    //! Lookup table to translate Sony camera settings dynamic range optimizer mode values to readable labels
    extern const TagDetails sonyDynamicRangeOptimizerMode[] = {
        { 0,    N_("Off")            },
        { 1,    N_("Standard")       },
        { 2,    N_("Advanced Auto")  },
        { 3,    N_("Advanced Level") },
        { 4097, N_("Auto")           }
    };

    //! Lookup table to translate Sony camera settings creative style values to readable labels
    extern const TagDetails sonyCreativeStyle[] = {
        { 1,    N_("Standard")            },
        { 2,    N_("Vivid")               },
        { 3,    N_("Portrait")            },
        { 4,    N_("Landscape")           },
        { 5,    N_("Sunset")              },
        { 6,    N_("Night View/Portrait") },
        { 8,    N_("Black & White")       },
        { 9,    N_("Adobe RGB")           },
        { 11,   N_("Neutral")             },
        { 12,   N_("Clear")               },
        { 13,   N_("Deep")                },
        { 14,   N_("Light")               },
        { 15,   N_("Autumn")              },
        { 16,   N_("Sepia")               }
    };

    //! Lookup table to translate Sony camera settings flash mode values to readable labels
    extern const TagDetails sonyFlashMode[] = {
        { 0, N_("ADI") },
        { 1, N_("TTL") },
    };

    //! Lookup table to translate Sony camera settings priority setup shutter release values to readable labels
    extern const TagDetails sonyPrioritySetupShutterRelease[] = {
        { 0, N_("AF") },
        { 1, N_("Release") },
    };

    //! Lookup table to translate Sony AF illuminator values to readable labels
    extern const TagDetails sonyAFIlluminator[] = {
        { 0, N_("Auto") },
        { 1, N_("Off")  }
    };

    //! Lookup table to translate Sony AF with shutter values to readable labels
    extern const TagDetails sonyAFWithShutter[] = {
        { 0, N_("On") },
        { 1, N_("Off")  }
    };

    //! Lookup table to translate Sony camera settings image style values to readable labels
    extern const TagDetails sonyImageStyle[] = {
        { 1,    N_("Standard")      },
        { 2,    N_("Vivid")         },
        { 9,    N_("Adobe RGB")     },
        { 11,   N_("Neutral")       },
        { 129,  N_("StyleBox1")     },
        { 130,  N_("StyleBox1")     },
        { 131,  N_("StyleBox1")     }
    };

    //! Lookup table to translate Sony camera settings exposure program values to readable labels
    extern const TagDetails sonyExposureProgram[] = {
        { 0,    N_("Auto")                      },
        { 1,    N_("Manual")                    },
        { 2,    N_("Program AE")                },
        { 3,    N_("Aperture-priority AE")      },
        { 4,    N_("Shutter speed priority AE") },
        { 8,    N_("Program Shift A")           },
        { 9,    N_("Program Shift S")           },
        { 16,    N_("Portrait")                  },
        { 17,   N_("Sports")                    },
        { 18,   N_("Sunset")                    },
        { 19,   N_("Night Portrait")            },
        { 20,   N_("Landscape")                 },
        { 21,   N_("Macro")                     },
        { 35,   N_("Auto No Flash")             }
    };

    //! Lookup table to translate Sony camera settings image size values to readable labels
    extern const TagDetails sonyImageSize[] = {
        { 1, N_("Large ") },
        { 2, N_("Medium") },
        { 3, N_("Small")  }
    };

    //! Lookup table to translate Sony aspect ratio values to readable labels
    extern const TagDetails sonyAspectRatio[] = {
        { 1, "3:2"   },
        { 2, "16:9"  }
    };

    //! Lookup table to translate Sony camera settings quality values to readable labels
    extern const TagDetails sonyQualityCS[] = {
        { 0,   N_("RAW ")       },
        { 2,   N_("CRAW ")      },
        { 16,  N_("Extra Fine") },
        { 32,  N_("Fine")       },
        { 34,  N_("RAW+JPEG")   },
        { 35,  N_("CRAW+JPEG")  },
        { 48,  N_("Standard")   }
    };

    //! Lookup table to translate Sony exposure level increments values to readable labels
    extern const TagDetails sonyExposureLevelIncrements[] = {
        { 33, "1/3 EV" },
        { 50, "1/2 EV" }
    };

    // Sony Camera Settings Tag Info
    // NOTE: all are for A200, A230, A300, A350, A700, A850 and A900 Sony model excepted
    // some entries which are only relevant with A700.

    const TagInfo SonyMakerNote::tagInfoCs_[] = {
        // NOTE: A700 only
        TagInfo(0x0004, "DriveMode", N_("Drive Mode"),
                N_("Drive Mode"),
                sony1CsIfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyDriveModeStd)),
        // NOTE: A700 only
        TagInfo(0x0006, "WhiteBalanceFineTune", N_("White Balance Fine Tune"),
                N_("White Balance Fine Tune"),
                sony1CsIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x0016, "FocusMode", N_("Focus Mode"),
                N_("Focus Mode"),
                sony1CsIfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyFocusMode)),
        TagInfo(0x0017, "AFAreaMode", N_("AF Area Mode"),
                N_("AF Area Mode"),
                sony1CsIfdId, makerTags, unsignedShort, printMinoltaSonyAFAreaMode),
        TagInfo(0x0018, "LocalAFAreaPoint", N_("Local AF Area Point"),
                N_("Local AF Area Point"),
                sony1CsIfdId, makerTags, unsignedShort, printMinoltaSonyLocalAFAreaPoint),
        TagInfo(0x0021, "MeteringMode", N_("Metering Mode"),
                N_("Metering Mode"),
                sony1CsIfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyMeteringMode)),
        TagInfo(0x0022, "ISOSetting", N_("ISO Setting"),
                N_("ISO Setting"),
                sony1CsIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x0024, "DynamicRangeOptimizerMode", N_("Dynamic Range Optimizer Mode"),
                N_("Dynamic Range Optimizer Mode"),
                sony1CsIfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyDynamicRangeOptimizerMode)),
        TagInfo(0x0025, "DynamicRangeOptimizerLevel", N_("Dynamic Range Optimizer Level"),
                N_("Dynamic Range Optimizer Level"),
                sony1CsIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x0026, "CreativeStyle", N_("Creative Style"),
                N_("Creative Style"),
                sony1CsIfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyCreativeStyle)),
        TagInfo(0x0028, "Sharpness", N_("Sharpness"),
                N_("Sharpness"),
                sony1CsIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x0029, "Contrast", N_("Contrast"),
                N_("Contrast"),
                sony1CsIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x0030, "Saturation", N_("Saturation"),
                N_("Saturation"),
                sony1CsIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x0031, "ZoneMatchingValue", N_("Zone Matching Value"),
                N_("Zone Matching Value"),
                sony1CsIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x0034, "Brightness", N_("Brightness"),
                N_("Brightness"),
                sony1CsIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x0035, "FlashMode", N_("FlashMode"),
                N_("FlashMode"),
                sony1CsIfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyFlashMode)),
        // NOTE: A700 only
        TagInfo(0x0040, "PrioritySetupShutterRelease", N_("Priority Setup Shutter Release"),
                N_("Priority Setup Shutter Release"),
                sony1CsIfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyPrioritySetupShutterRelease)),
        // NOTE: A700 only
        TagInfo(0x0041, "AFIlluminator", N_("AF Illuminator"),
                N_("AF Illuminator"),
                sony1CsIfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyAFIlluminator)),
        // NOTE: A700 only
        TagInfo(0x0042, "AFWithShutter", N_("AF With Shutter"),
                N_("AF With Shutter"),
                sony1CsIfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyAFWithShutter)),
        // NOTE: A700 only
        TagInfo(0x0043, "LongExposureNoiseReduction", N_("Long Exposure Noise Reduction"),
                N_("Long Exposure Noise Reduction"),
                sony1CsIfdId, makerTags, unsignedShort, printMinoltaSonyBoolValue),
        // NOTE: A700 only
        TagInfo(0x0044, "HighISONoiseReduction", N_("High ISO NoiseReduction"),
                N_("High ISO NoiseReduction"),
                sony1CsIfdId, makerTags, unsignedShort, printValue),
        // NOTE: A700 only
        TagInfo(0x0045, "ImageStyle", N_("Image Style"),
                N_("Image Style"),
                sony1CsIfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyImageStyle)),
        TagInfo(0x0060, "ExposureProgram", N_("Exposure Program"),
                N_("Exposure Program"),
                sony1CsIfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyExposureProgram)),
        TagInfo(0x0061, "ImageStabilization", N_("Image Stabilization"),
                N_("Image Stabilization"),
                sony1CsIfdId, makerTags, unsignedShort, printMinoltaSonyBoolValue),
        TagInfo(0x0063, "Rotation", N_("Rotation"),
                N_("Rotation"),
                sony1CsIfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyRotation)),
        TagInfo(0x0084, "SonyImageSize", N_("Sony Image Size"),
                N_("Sony Image Size"),
                sony1CsIfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyImageSize)),
        TagInfo(0x0085, "AspectRatio", N_("Aspect Ratio"),
                N_("Aspect Ratio"),
                sony1CsIfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyAspectRatio)),
        TagInfo(0x0086, "Quality", N_("Quality"),
                N_("Quality"),
                sony1CsIfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyQualityCS)),
        TagInfo(0x0088, "ExposureLevelIncrements", N_("Exposure Level Increments"),
                N_("Exposure Level Increments"),
                sony1CsIfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyExposureLevelIncrements)),
        // End of list marker
        TagInfo(0xffff, "(UnknownSony1CsTag)", "(UnknownSony1CsTag)",
                N_("Unknown Sony1 Camera Settings tag"),
                sony1CsIfdId, makerTags, invalidTypeId, printValue)
    };

    const TagInfo* SonyMakerNote::tagListCs()
    {
        return tagInfoCs_;
    }

    // -- Sony camera settings ---------------------------------------------------------------

    // Sony Camera Settings Tag Version 2 Info
    // NOTE: all are for A330, A380 Sony model

    const TagInfo SonyMakerNote::tagInfoCs2_[] = {

        TagInfo(0x0016, "FocusMode", N_("Focus Mode"),
                N_("Focus Mode"),
                sony1Cs2IfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyFocusMode)),
        TagInfo(0x0017, "AFAreaMode", N_("AF Area Mode"),
                N_("AF Area Mode"),
                sony1Cs2IfdId, makerTags, unsignedShort, printMinoltaSonyAFAreaMode),
        TagInfo(0x0018, "LocalAFAreaPoint", N_("Local AF Area Point"),
                N_("Local AF Area Point"),
                sony1Cs2IfdId, makerTags, unsignedShort, printMinoltaSonyLocalAFAreaPoint),
        TagInfo(0x0019, "MeteringMode", N_("Metering Mode"),
                N_("Metering Mode"),
                sony1Cs2IfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyMeteringMode)),
        TagInfo(0x0020, "ISOSetting", N_("ISO Setting"),
                N_("ISO Setting"),
                sony1Cs2IfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x0022, "DynamicRangeOptimizerMode", N_("Dynamic Range Optimizer Mode"),
                N_("Dynamic Range Optimizer Mode"),
                sony1Cs2IfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyDynamicRangeOptimizerMode)),
        TagInfo(0x0023, "DynamicRangeOptimizerLevel", N_("Dynamic Range Optimizer Level"),
                N_("Dynamic Range Optimizer Level"),
                sony1Cs2IfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x0024, "CreativeStyle", N_("Creative Style"),
                N_("Creative Style"),
                sony1Cs2IfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyCreativeStyle)),
        TagInfo(0x0025, "Sharpness", N_("Sharpness"),
                N_("Sharpness"),
                sony1Cs2IfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x0026, "Contrast", N_("Contrast"),
                N_("Contrast"),
                sony1Cs2IfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x0027, "Saturation", N_("Saturation"),
                N_("Saturation"),
                sony1Cs2IfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x0035, "FlashMode", N_("FlashMode"),
                N_("FlashMode"),
                sony1Cs2IfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyFlashMode)),
        TagInfo(0x0060, "ExposureProgram", N_("Exposure Program"),
                N_("Exposure Program"),
                sony1Cs2IfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyExposureProgram)),
        TagInfo(0x0063, "Rotation", N_("Rotation"),
                N_("Rotation"),
                sony1Cs2IfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyRotation)),
        TagInfo(0x0084, "SonyImageSize", N_("Sony Image Size"),
                N_("Sony Image Size"),
                sony1Cs2IfdId, makerTags, unsignedShort, EXV_PRINT_TAG(sonyImageSize)),
        // End of list marker
        TagInfo(0xffff, "(UnknownSony1Cs2Tag)", "(UnknownSony1Cs2Tag)",
                N_("Unknown Sony1 Camera Settings 2 tag"),
                sony1Cs2IfdId, makerTags, invalidTypeId, printValue)
    };

    const TagInfo* SonyMakerNote::tagListCs2()
    {
        return tagInfoCs2_;
    }
}                                       // namespace Exiv2
