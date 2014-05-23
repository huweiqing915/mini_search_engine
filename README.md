mini_search_engine
==================

server.exe:服务器的启动程序，可以与客户端连接 TCPSocket.cpp test_socket_client.cpp test_socket_server.cpp（tcp.make）

pagelib.exe:预处理，生成网页库 同时生成文档id-文档偏移量-文档大小的索引	PageLib.cpp, test_pagelib.cpp (pagelib.make)

offset.exe:预处理，测试偏移量索引是否正确 test_offset.cpp (offset.make)

pagesement.exe:测试网页切词，取topk PageSegment.cpp test_pagesegment.cpp (pagesegment.make)

delpage.exe:网页去重（指纹法，即取词频最多前10个）  PageSegment.cpp test_delpage.cpp (delpage.make)

remove_repeat.exe:网页去重（上一个方法，封装起来了） Document.cpp RemoveRepeat.cpp test_remove_page.cpp(removepage.make)

delduplicate.exe:网页去重（特征码法，取逗号的前后6个字节） FeatureCode.cpp DelDuplicate.cpp test_delduplicate.cpp(delduplicate.make)

index.exe:建立索引，以及权重索引 InverterIndex.cpp test_index.cpp(index.make)

search.exe:查询模块，输入查询词，由空间向量模型算出相似度，按相似度从大到小输出结果 Search.cpp test_search.cpp (search.make)

Engine.exe:




