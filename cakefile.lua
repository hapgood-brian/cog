--------------------------------------------------------------------------------
-- Create a new workspace.
--------------------------------------------------------------------------------

local eon = workspace:new'eon'--> Will create eon.xcworkspace

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile engine framework.
--------------------------------------------------------------------------------

local eng = eon:new'eon'--> Will create engine.xcproject
      eng : prefix'src/engine/include/xcode-prefix.pch'--> Precompiled header.
          : identifier'com.creepydollgames.eon'
          : organization'Brian Hapgood'
          : include'src/engine/include'--> scan this for headers.
          : sources'src/engine/src'--> Scan this for sources.
          : target'framework'
          : team'HE96RQ5ZY9'
          : lang'c++17'

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile startup code.
--------------------------------------------------------------------------------

local start = eon:new'start'
      start : include'src/com/start/include'
            : sources'src/com/start/src'
            : target'static'
            : lang'c++17'

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile startup code.
--------------------------------------------------------------------------------

local pal = eon:new'pal'
      pal : include'src/com/start/include'
          : sources'src/com/start/src'
          : target'static'
          : lang'c++17'

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile application.
--------------------------------------------------------------------------------

local app = eon:new'cog'
      app : identifier'com.creepydollgames.cog'
          : organization'Brian Hapgood'
          : include'src/app/include'
          : sources'src/app/src'
          : team'HE96RQ5ZY9'
          : target'app'
          : lang'c++17'

--------------------------------------------------------------------------------
-- Save all projects to tmp directory.
--------------------------------------------------------------------------------

return platform.save( eon, 'tmp' )
