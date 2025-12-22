#include <iostream>
#include <cstdlib>
#include <string>
#include <curl/curl.h>
using namespace std;
const string APIKEY = "YOUR_KEY_HERE";
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    ((string*)userp)->append((char*)contents, totalSize);
    return totalSize;
}
// IF YOU HAVE BETTER IDEAS FOR NAMING CONVENTIONS THEN YOU SHOULD IMPLEMENT THEM!!
class Stock {
    public:
        string name;
        int price; // Implement later
        string ReturnData;        // Shrimply for testing purposes
        void Init(){
        curl_global_init(CURL_GLOBAL_DEFAULT);
        CURL* handler = curl_easy_init();
        string link = "https://yahoo-finance15.p.rapidapi.com/api/v1/markets/search?search=" + name;
        string APIKEYWrit = "x-rapidapi-key: " + APIKEY;
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, APIKEYWrit.c_str());
        headers = curl_slist_append(headers, "x-rapidapi-host: yahoo-finance15.p.rapidapi.com");
        curl_easy_setopt(handler, CURLOPT_URL, link.c_str());
        curl_easy_setopt(handler, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(handler, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(handler, CURLOPT_WRITEDATA, &ReturnData);
        CURLcode res = curl_easy_perform(handler);
        if (res != CURLE_OK) {
            cerr << "curl error: " << curl_easy_strerror(res) << endl;
        }
        // tbh we can probs get rid of this entire part cuz like who cares about performance and memory clearing
        curl_slist_free_all(headers);
        curl_easy_cleanup(handler);
        curl_global_cleanup();
        }
};
int main() {
    string ticker;
    printf("Please enter a Stock Ticker Symbol!\n");
    cin >> ticker;
    Stock newRequest;
    newRequest.name = ticker;
    newRequest.Init();
    cout << newRequest.ReturnData << endl;
} 