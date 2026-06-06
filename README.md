# Countries

An Unreal Engine plugin that gets country data from the [REST Countries API](https://restcountries.com) and makes it available at both runtime and editor time, including flag and coat-of-arms images.

## Features

- Export every country as a `UCTCountryPrimaryDataAsset`, downloading the flag and coat-of-arms PNGs as `UTexture2D` assets.
- Blueprint async node `Fetch All Countries`.
- Blueprint library to open a country's Google Maps / OpenStreetMap page, flag, or coat of arms in the browser.

<p align="center">
  <img width="480" alt="Kenya exported as a data asset" src="https://github.com/user-attachments/assets/c19444e0-023e-470f-86ad-e8873e41c44e" />
  <br />
  <em>Kenya exported as a data asset</em>
</p>

## Generating Data Assets

In the editor, use **Tools → Countries → Generate Countries Data Assets**. The tool gets the data and creates one data asset per country.

## Settings

Configurable under **Project Settings → Editor → Countries Editor** (`UCTCountriesDeveloperSettings`):

| Setting | Default | Description |
| --- | --- | --- |
| `BasePackagePath` | `/Game/Countries` | Where generated assets are saved. |
| `FlagTexturePrefix` | `T_Flag_` | Prefix for flag textures. |
| `CoatOfArmsTexturePrefix` | `T_CoatOfArms_` | Prefix for coat-of-arms textures. |
| `MaxConcurrentDownloads` | `6` | Max simultaneous image downloads during export. |

## Blueprint API

- **Fetch All Countries**: async node returning the country array if successful.
- **Open Country In Google Maps / OpenStreetMaps / Maps**: launch the country's map URL.
- **Open Flag / Coat Of Arms In Browser**: launch the country's image URL.

## Notes

Country data is from the REST Countries API. Attribution and licensing for that data and for downloaded flag/coat-of-arms images follow the API's terms.
