# GNU Make project makefile autogenerated by Premake
ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifndef CC
  CC = gcc
endif

ifndef CXX
  CXX = g++
endif

ifndef AR
  AR = ar
endif

ifeq ($(config),debug)
  OBJDIR     = obj/Debug/box2d
  TARGETDIR  = ../../depends/box2D/build
  TARGET     = $(TARGETDIR)/libbox2d.a
  DEFINES   += 
  INCLUDES  += -I../../depends/box2d/include
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) 
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -s
  LIBS      += 
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LDDEPS    += 
  LINKCMD    = $(AR) -rcs $(TARGET) $(OBJECTS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),release)
  OBJDIR     = obj/Release/box2d
  TARGETDIR  = ../../depends/box2D/build
  TARGET     = $(TARGETDIR)/libbox2d.a
  DEFINES   += 
  INCLUDES  += -I../../depends/box2d/include
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) 
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -s
  LIBS      += 
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LDDEPS    += 
  LINKCMD    = $(AR) -rcs $(TARGET) $(OBJECTS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

OBJECTS := \
	$(OBJDIR)/b2BlockAllocator.o \
	$(OBJDIR)/b2Body.o \
	$(OBJDIR)/b2BroadPhase.o \
	$(OBJDIR)/b2CircleContact.o \
	$(OBJDIR)/b2CircleShape.o \
	$(OBJDIR)/b2CollideCircle.o \
	$(OBJDIR)/b2CollidePolygon.o \
	$(OBJDIR)/b2Collision.o \
	$(OBJDIR)/b2Contact.o \
	$(OBJDIR)/b2ContactManager.o \
	$(OBJDIR)/b2ContactSolver.o \
	$(OBJDIR)/b2Distance.o \
	$(OBJDIR)/b2DistanceJoint.o \
	$(OBJDIR)/b2DynamicTree.o \
	$(OBJDIR)/b2Fixture.o \
	$(OBJDIR)/b2FrictionJoint.o \
	$(OBJDIR)/b2GearJoint.o \
	$(OBJDIR)/b2Island.o \
	$(OBJDIR)/b2Joint.o \
	$(OBJDIR)/b2LineJoint.o \
	$(OBJDIR)/b2Math.o \
	$(OBJDIR)/b2MouseJoint.o \
	$(OBJDIR)/b2PolygonAndCircleContact.o \
	$(OBJDIR)/b2PolygonContact.o \
	$(OBJDIR)/b2PolygonShape.o \
	$(OBJDIR)/b2PrismaticJoint.o \
	$(OBJDIR)/b2PulleyJoint.o \
	$(OBJDIR)/b2RevoluteJoint.o \
	$(OBJDIR)/b2Settings.o \
	$(OBJDIR)/b2StackAllocator.o \
	$(OBJDIR)/b2TimeOfImpact.o \
	$(OBJDIR)/b2TOISolver.o \
	$(OBJDIR)/b2WeldJoint.o \
	$(OBJDIR)/b2World.o \
	$(OBJDIR)/b2WorldCallbacks.o \

RESOURCES := \

SHELLTYPE := msdos
ifeq (,$(ComSpec)$(COMSPEC))
  SHELLTYPE := posix
endif
ifeq (/bin,$(findstring /bin,$(SHELL)))
  SHELLTYPE := posix
endif

.PHONY: clean prebuild prelink

all: $(TARGETDIR) $(OBJDIR) prebuild prelink $(TARGET)
	@:

$(TARGET): $(GCH) $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking box2d
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning box2d
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild:
	$(PREBUILDCMDS)

prelink:
	$(PRELINKCMDS)

ifneq (,$(PCH))
$(GCH): $(PCH)
	@echo $(notdir $<)
	-$(SILENT) cp $< $(OBJDIR)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
endif

$(OBJDIR)/b2BlockAllocator.o: ../../depends/box2d/source/b2BlockAllocator.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2Body.o: ../../depends/box2d/source/b2Body.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2BroadPhase.o: ../../depends/box2d/source/b2BroadPhase.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2CircleContact.o: ../../depends/box2d/source/b2CircleContact.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2CircleShape.o: ../../depends/box2d/source/b2CircleShape.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2CollideCircle.o: ../../depends/box2d/source/b2CollideCircle.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2CollidePolygon.o: ../../depends/box2d/source/b2CollidePolygon.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2Collision.o: ../../depends/box2d/source/b2Collision.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2Contact.o: ../../depends/box2d/source/b2Contact.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2ContactManager.o: ../../depends/box2d/source/b2ContactManager.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2ContactSolver.o: ../../depends/box2d/source/b2ContactSolver.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2Distance.o: ../../depends/box2d/source/b2Distance.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2DistanceJoint.o: ../../depends/box2d/source/b2DistanceJoint.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2DynamicTree.o: ../../depends/box2d/source/b2DynamicTree.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2Fixture.o: ../../depends/box2d/source/b2Fixture.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2FrictionJoint.o: ../../depends/box2d/source/b2FrictionJoint.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2GearJoint.o: ../../depends/box2d/source/b2GearJoint.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2Island.o: ../../depends/box2d/source/b2Island.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2Joint.o: ../../depends/box2d/source/b2Joint.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2LineJoint.o: ../../depends/box2d/source/b2LineJoint.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2Math.o: ../../depends/box2d/source/b2Math.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2MouseJoint.o: ../../depends/box2d/source/b2MouseJoint.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2PolygonAndCircleContact.o: ../../depends/box2d/source/b2PolygonAndCircleContact.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2PolygonContact.o: ../../depends/box2d/source/b2PolygonContact.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2PolygonShape.o: ../../depends/box2d/source/b2PolygonShape.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2PrismaticJoint.o: ../../depends/box2d/source/b2PrismaticJoint.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2PulleyJoint.o: ../../depends/box2d/source/b2PulleyJoint.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2RevoluteJoint.o: ../../depends/box2d/source/b2RevoluteJoint.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2Settings.o: ../../depends/box2d/source/b2Settings.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2StackAllocator.o: ../../depends/box2d/source/b2StackAllocator.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2TimeOfImpact.o: ../../depends/box2d/source/b2TimeOfImpact.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2TOISolver.o: ../../depends/box2d/source/b2TOISolver.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2WeldJoint.o: ../../depends/box2d/source/b2WeldJoint.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2World.o: ../../depends/box2d/source/b2World.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/b2WorldCallbacks.o: ../../depends/box2d/source/b2WorldCallbacks.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)