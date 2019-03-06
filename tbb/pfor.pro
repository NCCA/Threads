TARGET=pfor
SOURCES += pfor.cpp
CONFIG+=c++14
# at home I install tbb into home dir you may need to change paths
# also add to runtime path
macx:INCLUDEPATH += $(HOME)/tbb/tbb2019_20190206oss/include
macx:INCLUDEPATH += $(HOME)/tbb/tbb2019_20190206oss/include
macx:INCLUDEPATH += $(HOME)/tbb/pstl2019_20190206oss/include
macx:LIBS += -Wl,-rpath,$(HOME)/tbb/tbb2019_20190206oss/lib -L$(HOME)/tbb/tbb2019_20190206oss/lib -ltbb
CONFIG-=app_bundle
