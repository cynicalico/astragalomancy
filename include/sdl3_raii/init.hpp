#pragma once

#include <string>

namespace sdl3 {
enum class AppType {
    Game,
    MediaPlayer,
    Application,
};

struct AppInfo {
    /**
     * `SDL_PROP_APP_METADATA_NAME_STRING`: The human-readable name of the
     * application, like "My Game 2: Bad Guy's Revenge!". This will show up
     * anywhere the OS shows the name of the application separately from window
     * titles, such as volume control applets, etc.
     */
    std::string name;

    /**
     * `SDL_PROP_APP_METADATA_VERSION_STRING`: The version of the app that is
     * running; there are no rules on format, so "1.0.3beta2" and "April 22nd,
     * 2024" and a git hash are all valid options.
     */
    std::string version;

    /**
     * `SDL_PROP_APP_METADATA_IDENTIFIER_STRING`: A unique string that
     * identifies this app. This must be in reverse-domain format, like
     * "com.example.mygame2". This string is used by desktop compositors to
     * identify and group windows together, as well as match applications with
     * associated desktop settings and icons. If you plan to package your
     * application in a container such as Flatpak, the app ID should match the
     * name of your Flatpak container as well.
     */
    std::string identifier;

    /**
     * `SDL_PROP_APP_METADATA_CREATOR_STRING`: The human-readable name of the
     * creator/developer/maker of this app, like "MojoWorkshop, LLC"
     */
    std::string creator;

    /**
     * `SDL_PROP_APP_METADATA_COPYRIGHT_STRING`: The human-readable copyright
     * notice, like "Copyright (c) 2024 MojoWorkshop, LLC" or whatnot. Keep this
     * to one line, don't paste a copy of a whole software license in here.
     */
    std::string copyright;

    /**
     * `SDL_PROP_APP_METADATA_URL_STRING`: A URL to the app on the web. Maybe a
     * product page, or a storefront, or even a GitHub repository, for user's
     * further information.
     */
    std::string url;

    /**
     * `SDL_PROP_APP_METADATA_TYPE_STRING`: The type of application this is.
     * Currently this string can be "game" for a video game, "mediaplayer" for a
     * media player, or generically "application" if nothing else applies.
     * Future versions of SDL might add new types. This defaults to
     * "application".
     */
    AppType type;
};

bool init(const AppInfo& app_info);

void exit();
} // namespace sdl3
