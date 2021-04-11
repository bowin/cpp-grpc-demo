# c++ grpc demo 
c++ grpc demo under MacOS
## env setup 

1. 安装依赖 protobuf, grpc

安装 protobuf 自动获得 protoc

安装 grpc 自动安装 openssl, grpc_cpp_plugin
   
2. 生成 proto 相关 cpp
- 生成 pb
  `protoc -I ./ --cpp_out=./ ./helloworld.proto`
- 生成 grpc
  ``protoc -I ./ --grpc_out=./ ./helloworld.proto --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ``
  
3. cmake 配置



