TARGET=pfor
SOURCES += $$PWD/pfor.cpp \
           $$PWD/OpenCL.cpp
HEADERS +=$$PWD/OpenCL.h
OTHER_FILES +=$$PWD/addOne.cl

CONFIG+=c++14
# at home I install tbb into home dir you may need to change paths
# also add to runtime path
macx:INCLUDEPATH += $(HOME)/tbb/tbb2019_20190206oss/include
macx:INCLUDEPATH += $(HOME)/tbb/tbb2019_20190206oss/include
macx:INCLUDEPATH += $(HOME)/tbb/pstl2019_20190206oss/include
macx:LIBS += -Wl,-rpath,$(HOME)/tbb/tbb2019_20190206oss/lib -L$(HOME)/tbb/tbb2019_20190206oss/lib -ltbb
macx:LIBS+= -framework OpenCL
macx:CONFIG-=app_bundle

linux*:INCLUDEPATH+=/public/devel/2018/tbb/include
linux*:LIBS += -Wl,-rpath,/public/devel/2018/tbb/lib/intel64/gcc4.7/ -L/public/devel/2018/tbb/lib/intel64/gcc4.7/ -ltbb
linux*:INCLUDEPATH+=/public/devel/2018/pstl/include
