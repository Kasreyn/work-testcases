#comment AXIS M4348/M4347-PLVE Fisheye Camera (Calamari)
{
  "input": {
    "dptz": false,
    "mirror": true,
    "rotation": [0],
    "res_smart": [3536,3232,2992,2880,2464,2160,2048,1920,1440,1024,960]
  },
  "hidden": {
  },
  "warp": {
    "dptz": true,
    "mirror": true,
    "rotation": [0]
  },
  "composite": {
    "dptz": false,
    "mirror": false,
    "rotation": [0]
  },
  "channels": {
     "1": { "/input": 1 },
     "2": { "/warp": 1, "source": 1 },
     "3": { "/warp": 2, "source": 1 },
     "4": { "/warp": 3, "source": 1 },
     "5": { "/warp": 4, "source": 1 },
     "6": { "/warp": 5, "source": 1 },
     "7": { "/warp": 6, "source": 1 },
     "8": { "/warp": 7, "source": 1 },
     "9": { "/composite": 1, "layout": "13:14" },
    "10": { "/composite": 2, "layout": "15,16:17,18" },
    "11": { "/composite": 3, "layout": "7:8" },
    "12": { "/composite": 4, "layout": "19,20" },
    "13": { "/hidden": 1, "hidden.type": "warp", "source": 1, "hidden.id":  8 },
    "14": { "/hidden": 2, "hidden.type": "warp", "source": 1, "hidden.id":  9 },
    "15": { "/hidden": 3, "hidden.type": "warp", "source": 1, "hidden.id": 10 },
    "16": { "/hidden": 4, "hidden.type": "warp", "source": 1, "hidden.id": 11 },
    "17": { "/hidden": 5, "hidden.type": "warp", "source": 1, "hidden.id": 12 },
    "18": { "/hidden": 6, "hidden.type": "warp", "source": 1, "hidden.id": 13 },
    "19": { "/hidden": 7, "hidden.type": "warp", "source": 1, "hidden.id": 14 },
    "20": { "/hidden": 8, "hidden.type": "warp", "source": 1, "hidden.id": 15 }
  },
  "legacy_fisheye": {
    #comment Leading ; means 'res_smart' is used for the first channel!
    "2464x2464": [
#comment PANORAMA 2:1
      ";2304x1152,1920x960,1280x640,640x320,,,,2304x864",
#comment PANORAMA 8:3 *** Remove before release ***
      ",,,,2304x864;",
#comment 4xVIEW 16:9
      "2304x1296,2048x1152,1920x1080,1280x720,1024x576,640x360,512x288,256x144",
#comment 4xVIEW 4:3 *** Remove before release ***
      ",,,,1920x1440,1440x1080,1280x960,640x480;;;;",
#comment 2xCORNER 2:1
      "2368x1184,1920x960,1280x640,640x320;;",
#comment DOUBLE PANORAMA 16:9
      "2304x1296,2048x1152,1920x1080,1280x720,1024x576,640x360,512x288,256x144",
#comment DOUBLE PANORAMA 4:3 *** Remove before release ***
      ",,,,2304x1728;;",
#comment DOUBLE CORNER 1:1
      "2304x2304,2048x2048,1536x1536,1024x1024,512x512;",
#comment CORRIDOR 16:9
      "2304x1296,2048x1152,1920x1080,1280x720,1024x576,640x360,512x288,256x144",
#comment CORRIDOR 4:3 *** Remove before release ***
      ",,,,2304x1728,2048x1536,1920x1440,1728x1296,1600x1200,1024x768,960x720,768x576,640x480,384x288,320x240"
    ],
    "2992x2992": [
#comment PANORAMA 2:1
      ";2560x1280,1920x960,1280x640,640x320",
#comment PANORAMA 8:3 *** Remove before release ***
      ",,,,2560x960;",
#comment 4xVIEW 16:9
      "2560x1440,2304x1296,2048x1152,1920x1080,1280x720,1024x576,640x360,512x288,256x144",
#comment 4xVIEW 4:3 *** Remove before release ***
      ",,,,2560x1920,1920x1440,1440x1080,1280x960,640x480;;;;",
#comment 2xCORNER 2:1
      "2368x1184,1920x960,1280x640,640x320;;",
#comment DOUBLE PANORAMA 16:9
      "2560x1440,2304x1296,2048x1152,1920x1080,1280x720,1024x576,640x360,512x288,256x144",
#comment DOUBLE PANORAMA 4:3 *** Remove before release ***
      ",,,,2560x1920;;",
#comment DOUBLE CORNER 1:1
      "2880x2880,2560x2560,2048x2048,1536x1536,1024x1024,512x512;",
#comment CORRIDOR 16:9
      "2560x1440,2304x1296,2048x1152,1920x1080,1280x720,1024x576,640x360,512x288,256x144",
#comment CORRIDOR 4:3 *** Remove before release ***
      ",,,,2560x1920,2432x1824,2304x1728,2048x1536,1920x1440,1728x1296,1600x1200,1024x768,960x720,768x576,640x480,384x288,320x240"
    ],
    "3536x3536": [
#comment PANORAMA 2:1
      ";2560x1280,1920x960,1280x640,640x320",
#comment PANORAMA 8:3 *** Remove before release ***
      ",,,,2560x960;",
#comment 4xVIEW 16:9
      "2560x1440,2304x1296,2048x1152,1920x1080,1280x720,1024x576,640x360,512x288,256x144",
#comment 4xVIEW 4:3 *** Remove before release ***
      ",,,,2560x1920,1920x1440,1440x1080,1280x960,640x480;;;;",
#comment 2xCORNER 2:1
      "2368x1184,1920x960,1280x640,640x320;;",
#comment DOUBLE PANORAMA 16:9
      "2560x1440,2304x1296,2048x1152,1920x1080,1280x720,1024x576,640x360,512x288,256x144",
#comment DOUBLE PANORAMA 4:3 *** Remove before release ***
      ",,,,2560x1920;;",
#comment DOUBLE CORNER 1:1
      "2880x2880,2560x2560,2048x2048,1536x1536,1024x1024,512x512;",
#comment CORRIDOR 16:9
      "2560x1440,2304x1296,2048x1152,1920x1080,1280x720,1024x576,640x360,512x288,256x144",
#comment CORRIDOR 4:3 *** Remove before release ***
      ",,,,2560x1920,2432x1824,2304x1728,2048x1536,1920x1440,1728x1296,1600x1200,1024x768,960x720,768x576,640x480,384x288,320x240"
    ]
  }
}
