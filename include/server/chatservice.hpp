#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include "redis.hpp"
#include "json.hpp"
#include "usermodel.hpp"
#include "friendmodel.hpp"
#include "offlinemsgmodel.hpp"
#include "groupmodel.hpp"
#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include <mutex>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using json = nlohmann::json;

// 表示处理消息的事件回调方法类型
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;

// 单例模式设计 聊天服务业务类
class ChatService
{
public:
    // 获取单例对象的接口函数
    static ChatService* instance();
    // 获取消息对应的处理器
    MsgHandler getHandler(int msgid);
    // 处理登录业务
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理注册业务
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 一对一聊天业务
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 添加好友业务
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 创建群组业务
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 加入群组业务
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 群组聊天业务
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理注销业务
    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 服务器异常，业务重置方法
    void reset();
    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn);
    // 从redis消息队列中获取订阅的消息
    void handleRedisSubscribeMessage(int, string);
private:
    ChatService();
    
    // 存储消息id和其对应的业务处理方法
    unordered_map<int, MsgHandler> _msgHandlerMap;
    
    // 存储在线用户的通信连接
    unordered_map<int, TcpConnectionPtr> _userConnMap;
    
    // 用户连接的锁，保证 _userConnMap 的安全
    mutex _connMutex;

    // 数据操作类对象
    UserModel _userModel;
    OfflineMsgModel _offlineMsgModel;
    FriendModel _friendModel;
    GroupModel _groupModel;

    // redis 操作对象
    Redis _redis;
};

#endif