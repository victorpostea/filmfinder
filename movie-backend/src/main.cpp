#include <cstdlib>
#include <string>
#include "crow.h"
#include <cpr/cpr.h>

int main() {
    crow::SimpleApp app;

    // Root route with CORS header
    CROW_ROUTE(app, "/")([]() {
        crow::response res("Hello, Crow!");
        res.add_header("Access-Control-Allow-Origin", "*");
        return res;
    });

    // OPTIONS route for /movies
    CROW_ROUTE(app, "/movies")
        .methods(crow::HTTPMethod::OPTIONS)
        ([](const crow::request&, crow::response& res) {
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
            res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
            res.code = 204; // No Content
            res.end();
        });

    // GET route for /movies (search endpoint)
    CROW_ROUTE(app, "/movies")
        .methods(crow::HTTPMethod::GET)
        ([](const crow::request& req) {
            auto query_params = crow::query_string(req.url_params);
            if (!query_params.get("query")) {
                crow::response res(400, "Missing 'query' parameter");
                res.add_header("Access-Control-Allow-Origin", "*");
                return res;
            }
            std::string title = query_params.get("query");

            const char* bearer_token = std::getenv("TMDB_BEARER_TOKEN");
            if (!bearer_token) {
                crow::response res(500, "Bearer Token not set");
                res.add_header("Access-Control-Allow-Origin", "*");
                return res;
            }
            std::string bearer_token_str = bearer_token;
            
            // Use CPR's Parameters to properly encode the query
            auto parameters = cpr::Parameters{{"query", title}};
            cpr::Header headers{{"Authorization", "Bearer " + bearer_token_str}, {"Accept", "application/json"}};
            
            cpr::Response r = cpr::Get(
                cpr::Url{"https://api.themoviedb.org/3/search/movie"},
                parameters,
                headers
            );
            
            if (r.status_code != 200) {
                crow::response res(500, "Failed to fetch data from TMDB API");
                res.add_header("Access-Control-Allow-Origin", "*");
                return res;
            }
            crow::response res(r.text);
            res.set_header("Content-Type", "application/json");
            res.add_header("Access-Control-Allow-Origin", "*");
            return res;
        });

    // OPTIONS route for /discover
    CROW_ROUTE(app, "/discover")
        .methods(crow::HTTPMethod::OPTIONS)
        ([](const crow::request&, crow::response& res) {
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
            res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
            res.code = 204; // No Content
            res.end();
        });

    // GET route for /discover (recommendation endpoint)
    CROW_ROUTE(app, "/discover")
        .methods(crow::HTTPMethod::GET)
        ([](const crow::request& req) {
            auto query_params = crow::query_string(req.url_params);
            if (!query_params.get("genres")) {
                crow::response res(400, "Missing 'genres' parameter");
                res.add_header("Access-Control-Allow-Origin", "*");
                return res;
            }
            std::string genres = query_params.get("genres");

            const char* bearer_token = std::getenv("TMDB_BEARER_TOKEN");
            if (!bearer_token) {
                crow::response res(500, "Bearer Token not set");
                res.add_header("Access-Control-Allow-Origin", "*");
                return res;
            }
            std::string bearer_token_str = bearer_token;
            // Construct the TMDB discover URL with genre filter and sort by popularity
            std::string tmdb_url = "https://api.themoviedb.org/3/discover/movie?with_genres=" 
                                   + genres + "&sort_by=popularity.desc";
            cpr::Header headers{{"Authorization", "Bearer " + bearer_token_str}, {"Accept", "application/json"}};
            cpr::Response r = cpr::Get(cpr::Url{tmdb_url}, headers);
            if (r.status_code != 200) {
                crow::response res(500, "Failed to fetch data from TMDB API");
                res.add_header("Access-Control-Allow-Origin", "*");
                return res;
            }
            crow::response res(r.text);
            res.set_header("Content-Type", "application/json");
            res.add_header("Access-Control-Allow-Origin", "*");
            return res;
        });

    // This mapping converts genre names to TMDB genre IDs
    std::unordered_map<std::string, std::string> genreNameToId = {
        {"action", "28"},
        {"adventure", "12"},
        {"animation", "16"},
        {"comedy", "35"},
        {"crime", "80"},
        {"documentary", "99"},
        {"drama", "18"},
        {"family", "10751"},
        {"fantasy", "14"},
        {"history", "36"},
        {"horror", "27"},
        {"music", "10402"},
        {"mystery", "9648"},
        {"romance", "10749"},
        {"sci_fi", "878"},
        {"thriller", "53"},
        {"war", "10752"},
        {"western", "37"}
    };

    // OPTIONS route for /discover/genre
    CROW_ROUTE(app, "/discover/genre")
        .methods(crow::HTTPMethod::OPTIONS)
        ([](const crow::request&, crow::response& res) {
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Access-Control-Allow-Methods", "GET, OPTIONS");
            res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
            res.code = 204; // No Content
            res.end();
        });

    // GET route for /discover/genre (discover by genre name endpoint)
    CROW_ROUTE(app, "/discover/genre")
        .methods(crow::HTTPMethod::GET)
        ([&genreNameToId](const crow::request& req) {
            auto query_params = crow::query_string(req.url_params);
            if (!query_params.get("name")) {
                crow::response res(400, "Missing 'name' parameter");
                res.add_header("Access-Control-Allow-Origin", "*");
                return res;
            }
            std::string genreName = query_params.get("name");
            
            // Convert genre name to TMDB genre ID
            std::string genreId;
            if (genreNameToId.find(genreName) != genreNameToId.end()) {
                genreId = genreNameToId[genreName];
            } else {
                crow::response res(400, "Invalid genre name");
                res.add_header("Access-Control-Allow-Origin", "*");
                return res;
            }

            const char* bearer_token = std::getenv("TMDB_BEARER_TOKEN");
            if (!bearer_token) {
                crow::response res(500, "Bearer Token not set");
                res.add_header("Access-Control-Allow-Origin", "*");
                return res;
            }
            std::string bearer_token_str = bearer_token;
            
            // Construct the TMDB discover URL with genre filter and sort by popularity
            std::string tmdb_url = "https://api.themoviedb.org/3/discover/movie?with_genres="
                                + genreId + "&sort_by=popularity.desc";
            
            cpr::Header headers{{"Authorization", "Bearer " + bearer_token_str}, {"Accept", "application/json"}};
            cpr::Response r = cpr::Get(cpr::Url{tmdb_url}, headers);
            
            if (r.status_code != 200) {
                crow::response res(500, "Failed to fetch data from TMDB API");
                res.add_header("Access-Control-Allow-Origin", "*");
                return res;
            }
            
            crow::response res(r.text);
            res.set_header("Content-Type", "application/json");
            res.add_header("Access-Control-Allow-Origin", "*");
            return res;
        });

    app.port(8080).multithreaded().run();
}
