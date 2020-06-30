#pragma once

#include "Poco/URI.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include <map>

using Poco::URI;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPServerResponse;

namespace dxlib {

///-------------------------------------------------------------------------------------------------
/// <summary> 解析一个URI里面的Query参数，原本是一个Vector<pair>的. </summary>
///
/// <remarks> Dx, 2020/3/13. </remarks>
///-------------------------------------------------------------------------------------------------
class ParserQueryParameters
{
  public:
    ParserQueryParameters(const URI& uri)
    {
        parameters = uri.getQueryParameters();

        //这里是默认它Vector里面的所有key没有重复的
        for (size_t i = 0; i < parameters.size(); i++) {
            keyValue[parameters[i].first] = parameters[i].second;
            keyValues[parameters[i].first].push_back(parameters[i].second);
        }
    }
    ~ParserQueryParameters() {}

    ///-------------------------------------------------------------------------------------------------
    /// <summary>
    /// conan库API的URI中的QueryParameters，这是简单一个拷贝.
    /// </summary>
    ///-------------------------------------------------------------------------------------------------
    Poco::URI::QueryParameters parameters;

    /// <summary> QueryParameters拆成键值对，如果key没有重复的那么直接使用它即可. </summary>
    std::map<std::string, std::string> keyValue;

    /// <summary>如果QueryParameters里有重复key，那么就可以使用这个. </summary>
    std::map<std::string, std::vector<std::string>> keyValues;

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 判断是否存在key,如果不存在那么直接返回错误. </summary>
    ///
    /// <remarks> Dx, 2020/3/18. </remarks>
    ///
    /// <param name="keys">     The keys. </param>
    /// <param name="response"> [out] The response. </param>
    ///
    /// <returns> 如果成功检测到存在key返回true. </returns>
    ///-------------------------------------------------------------------------------------------------
    bool checkKey(const std::vector<std::string>& keys, HTTPServerResponse* response = nullptr)
    {
        for (size_t i = 0; i < keys.size(); i++) {
            //如果检测出来一个不存在那么就返回false
            if (!checkKey(keys[i], response)) {
                return false;
            }
        }
        return true;
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 判断是否存在key,如果不存在那么直接返回错误. </summary>
    ///
    /// <remarks> Dx, 2020/3/18. </remarks>
    ///
    /// <param name="key">      The key. </param>
    /// <param name="response"> [out] 可以用来自动返回response. </param>
    ///
    /// <returns> 如果成功检测到存在key返回true. </returns>
    ///-------------------------------------------------------------------------------------------------
    bool checkKey(const std::string& key, HTTPServerResponse* response = nullptr)
    {
        //std::find_if(keyValue.begin(), keyValue.end(), [key](const auto& itr) { return itr.second == key; });

        // 如果存在这个key
        if (keyValue.find(key) != keyValue.end()) {
            return true;
        }
        else {
            if (response != nullptr) {
                response->setStatus(HTTPResponse::HTTP_BAD_REQUEST);
                response->setContentType("text/plain");
                std::ostream& out = response->send();
                out << "未包含必须字段:" << key;
            }
            return false;
        }
    }

  private:
};

} // namespace dxlib
