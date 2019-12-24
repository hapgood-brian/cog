//------------------------------------------------------------------------------
//       Copyright 2015-2016 Superluminal Games LLC. All rights reserved.
//
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
// NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define VC_EXTRALEAN
#define NOMINMAX

#include<windows.h>
#include<Shlobj.h>
#include<direct.h>
#include<eon.h>

using namespace EON;
using namespace ui;

namespace{
  u32    s_uMainTID = Thread::tid();
  string s_sFallbackDir;
  string s_sPackagePath;
  char   s_aTemp4K[4096];
  string s_stdErr;
  string s_stdOut;
}

//================================================|=============================
//IEngine:{                                       |
  //Externs:{                                     |

    extern bool g_bTerminate;
    extern f32  g_fScale;
    extern f32  g_fH;
    extern f32  g_fW;

  //}:                                            |
  //Private:{                                     |

    namespace{
      deque<std::function<void()>> anon_dLambdas;
      pt2 anon_tCursor;
    }

  //}:                                            |
  //Methods:{                                     |
    //[paths]:{                                   |
      //setPackagePath:{                          |

#ifdef __APPLE__
  #pragma mark - IEngine methods -
#endif

        void IEngine::setPackagePath( const string& dir ){
          if( dir.empty() ){
            s_sPackagePath.clear();
          }else{
            s_sPackagePath = dir;
            while( s_sPackagePath.back() == '/' ){
              s_sPackagePath.trim( 1 );
            }
          }
        }

      //}:                                        |
      //toResourcePath:{                          |

        string IEngine::toResourcePath(){
          string bundlePath = toBundlePath();
          bundlePath += "Contents/Resources/";
          if( dexists( bundlePath )){
            return bundlePath;
          }
          return nullptr;
        }

      //}:                                        |
      //toPackagePath:{                           |

        string IEngine::toPackagePath(){
          if( s_sPackagePath.empty() ){
            return string( "~/" );
          }
          return s_sPackagePath + "/";
        }

      //}:                                        |
      //toHelpersPath:{                           |

        string IEngine::toHelpersPath(){
          return toBundlePath() + "Contents/Helpers/";
        }

      //}:                                        |
      //toPluginsPath:{                           |

        string IEngine::toPluginsPath(){
          return toBundlePath() + "Contents/PlugIns/";
        }

      //}:                                        |
      //toBundlePath:{                            |

        string IEngine::toBundlePath(){
          static string s_sBundlePath;
          if( s_sBundlePath.empty() ){
            s_sBundlePath = args[ 0 ].path() + "/";
            s_sBundlePath.replace( "\\", "/" );
            s_sBundlePath.replace( "//", "/" );
          }
          return s_sBundlePath;
        }

      //}:                                        |
      //toEonPath:{                               |

        string IEngine::toEonPath(){
          string eonPath;
          if( !s_sPackagePath.empty() ){
            if(( '.' == *s_sPackagePath ) && ( '/' == s_sPackagePath[ 1 ])){
              eonPath += toResourcePath();
              eonPath += s_sPackagePath;
            }else{
              eonPath += s_sPackagePath;
            }
            eonPath += "/.eon/";
          }else{
            eonPath = toResourcePath();
          }
          return eonPath;
        }

      //}:                                        |
      //exit:{                                    |

        void IEngine::exit(){
          try{
            ::exit( 0 );
          }
          catch(...){
          }
        }

      //}:                                        |
    //}:                                          |
    //[ipc]:{                                     |
      //unshare:{                                 |

        bool IEngine::unshare( const string& key ){
          return false;
        }

      //}:                                        |
      //ssync:{                                   |

        bool IEngine::ssync( const string& key ){
          return false;
        }

      //}:                                        |
      //share:{                                   |

        cp IEngine::share( const gfc::string& path, const u64 req_bytes, const bool bServer ){
          return nullptr;
        }

      //}:                                        |
    //}:                                          |
    //[io]:{                                      |
      //saveFileSheet:{                           |

        bool IEngine::saveFileSheet( const strings& vExts
            , ccp pBaseDir
            , ccp pPrompt
            , ccp pTitle
            , const std::function<void( const string& path )>& in_onCompletion, const std::function<void()>& in_onCancel ){
          return false;
        }

      //}:                                        |
      //openFileSheet:{                           |

        bool IEngine::openFileSheet( const strings& vStrings
            , ccp pBaseDir
            , bool bFiles
            , bool bDirs
            , const std::function<void( const strings& paths )>& in_onCompletion, const std::function<void()>& in_onCancel ){
          return false;
        }

      //}:                                        |
      //tempPath:{                                |

        string IEngine::tempPath(){
          static string s_sTempPath;
          if( s_sTempPath.empty() ){
            GetTempPathA( e_dimof( s_aTemp4K ), s_aTemp4K );
            s_sTempPath  = string( s_aTemp4K ).os() + "\\";
            s_sTempPath.replace( "\\", "/" );
            s_sTempPath.replace( "//", "/" );
          }
          return s_sTempPath;
        }

      //}:                                        |
      //homePath:{                                |

        string IEngine::homePath(){
          static string s_sHomePath;
          if( s_sHomePath.empty() ){
            SHGetFolderPathA( NULL, CSIDL_PROFILE, NULL, 0, s_aTemp4K );
            string path = string( s_aTemp4K ) + '/';
            path.replace( "\\", "/" );
            path.replace( "//", "/" );
            s_sHomePath = path;
          }
          return s_sHomePath;
        }

      //}:                                        |
      //dexists:{                                 |

        bool IEngine::dexists( const string& path ){
          DWORD dwAttrib = GetFileAttributes( path.os() );
          return( dwAttrib != INVALID_FILE_ATTRIBUTES )&&( dwAttrib & FILE_ATTRIBUTE_DIRECTORY );
        }

      //}:                                        |
      //fexists:{                                 |

        bool IEngine::fexists( const string& path ){
          DWORD dwAttrib = GetFileAttributes( path.os() );
          if( dwAttrib == INVALID_FILE_ATTRIBUTES ){
            return false;
          }
          if( dwAttrib & FILE_ATTRIBUTE_DIRECTORY ){
            return false;
          }
          return true;
        }

      //}:                                        |
      //fopen:{                                   |

        FILE* IEngine::fopen( const string& path, ccp mode ){
          if( strchr( mode, 'w' )){
            md( path );
          }
          FILE* pFile = nullptr;
          const errno_t e = fopen_s( &pFile, path.os(), mode );
          if( !e ){
            return pFile;
          }
          return nullptr;
        }

      //}:                                        |
      //dir:{                                     |

        bool IEngine::dir( const string& path, const std::function<void( const string& dir
            , const string& name, const bool bIsDirectory )>& lambda ){
          WIN32_FIND_DATA fd;
          HANDLE hFind = FindFirstFile( (path + "/*.*").os(), &fd );
          if( hFind == INVALID_HANDLE_VALUE ){
            return false;
          }
          do{
            const bool bIsDirectory=( 0 != ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ));
            if( bIsDirectory ){
              if(( *fd.cFileName != '.' )||!strcmp( fd.cFileName, ".." )){
                lambda( path, fd.cFileName, true );
              }
            }else{
              lambda( path, fd.cFileName, false );
            }
          } while( FindNextFile( hFind, &fd ));
          if( GetLastError() != ERROR_NO_MORE_FILES ){
            FindClose( hFind );
            return false;
          }
          FindClose( hFind );
          return true;
        }

      //}:                                        |
    //}:                                          |
    //runOnMainThread:{                           |

      void IEngine::runOnMainThread( const std::function<void()>& lambda ){
        try{
          if( isMainThread() ){
            lambda();
          }else{
            anon_dLambdas.push( lambda );
          }
        }
        catch(...){
          e_errorf( 837447, "Unknown error caught." );
        }
      }

    //}:                                          |
    //setCursorShape:{                            |

      void IEngine::setCursorShape( const Cursors eCursor ){
        switch( eCursor ){
          case Cursors::kArrow:
            break;
          case Cursors::kIBeam:
            break;
          case Cursors::kCrossHair:
            break;
          case Cursors::kClosedHand:
            break;
          case Cursors::kOpenHand:
            break;
          case Cursors::kPointingHand:
            break;
          case Cursors::kResizeLeft:
            break;
          case Cursors::kResizeRight:
            break;
          case Cursors::kResizeLeftAndRight:
            break;
          case Cursors::kResizeUp:
            break;
          case Cursors::kResizeDown:
            break;
          case Cursors::kResizeUpAndDown:
            break;
          case Cursors::kDisappearingItem:
            break;
          case Cursors::kIBeamVertical:
            break;
          case Cursors::kNotAllowed:
            break;
          case Cursors::kDragLink:
            break;
          case Cursors::kDragCopy:
            break;
          case Cursors::kContextMenu:
            break;
        }
      }

    //}:                                          |
    //cursor:{                                    |

      bool IEngine::cursor( pt2& p ){
        p = anon_tCursor;
        p.scaleToClip( e_toScale2D() );
        p.x = (p.x+1.)*.5*g_fW;
        p.y = (p.y+1.)*.5*g_fH;
        return true;
      }

    //}:                                          |
    //cyView:{                                    |

      f32 IEngine::cyView(){
        return g_fH;
      }

    //}:                                          |
    //cxView:{                                    |

      f32 IEngine::cxView(){
        return g_fW;
      }

    //}:                                          |
    //ppi:{                                       |

      f32 IEngine::ppi(){
        return( 72.f * g_fScale );
      }

    //}:                                          |
    //setClipboardText:{                          |

      void IEngine::setClipboardText( const string& clipboardText ){
      }

    //}:                                          |
    //getClipboardText:{                          |

      string IEngine::getClipboardText(){
        return "";
      }

    //}:                                          |
    //deliverNotification:{                       |

      void IEngine::deliverNotification( const string& title, const string& message ){
        // TODO: Display message in notification center.
      }

    //}:                                          |
    //isMainThread:{                              |

      bool IEngine::isMainThread(){
        return( s_uMainTID == Thread::tid() );
      }

    //}:                                          |
    //areYouSure:{                                |

      bool IEngine::areYouSure( const string& title
          , const string& body
          , const std::function<void()>& in_onOK
          , const std::function<void()>& in_onCancel ){
        return false;
      }

    //}:                                          |
    //stderr:{                                    |

      string& IEngine::getStderr(){
        return s_stdErr;
      }

    //}:                                          |
    //stdout:{                                    |

      string& IEngine::getStdout(){
        return s_stdOut;
      }

    //}:                                          |
    //system:{                                    |

      bool IEngine::system( const string& cmd
          , const strings& cvArgs
          , const std::function<void()>& lambda ){
        const auto syscall=[=](){
          string arg;
          cvArgs.foreach( [&]( const string& cArg ){
            string s( cArg );
            s.replace( " ", "\\ " );
            arg += " ";
            arg += s;
          });
          system( e_xfs( "%s %s ", cmd.c_str(), arg.c_str() ));
        };
        if( lambda ){
          std::thread(
            [=](){
              syscall();
              lambda();
            }
          );
        }else{
          syscall();
        }
        return true;
      }

    //}:                                          |
    //spawn:{                                     |

      bool IEngine::spawn( const string& program
          , const strings& userArgs
          , const bool bBlocking
          , const std::function<void( const s32 )>& lambda ){
        if( !IEngine::isMainThread() ){
          return false;
        }
        const auto& onSpawn=[=]()->int{
          strings args;
          args.push( program );
          args.pushVector( userArgs );
          const int result = IPlugin::spawn( args );
          if( lambda ){
            lambda( result );
          }
          return result;
        };
        if( bBlocking ){
          return !onSpawn();
        }
        e_runAsync( onSpawn );
        return true;
      }

    //}:                                          |
    //exec:{                                      |

      bool IEngine::exec( const string& program
          , const strings& userArgs
          , const bool bBlocking
          , const std::function<void( const s32 )>& lambda ){
        // See also https://support.microsoft.com/en-us/kb/190351.
        PROCESS_INFORMATION pi;
        STARTUPINFO si;
        memset( &si, 0, sizeof( si ));
        memset( &pi, 0, sizeof( pi ));
        si.cb = sizeof( si );
        string arg;
        args.foreach( [&]( const string& s ){
          arg += " ";
          arg += s;
        });
        if( !CreateProcess(
              program,
              LPSTR( arg.c_str() ),
              NULL,
              NULL,
              FALSE,
              IsDebuggerPresent() ? 0 : CREATE_NO_WINDOW,
              NULL,
              NULL,
              &si,
              &pi )){
          #if e_compiling( debug )
            const DWORD dwError = GetLastError();
            if( dwError == ERROR_FILE_NOT_FOUND ){
              return false;
            }
            DEBUG_BREAK
          #endif
          return false;
        }
        if( bBlocking ){
          WaitForSingleObject( pi.hProcess, INFINITE );
          CloseHandle( pi.hProcess );
          CloseHandle( pi.hThread );
          if( lambda ){
            lambda( 0 );
          }
        }else{
          std::thread( [=](){
            while( WAIT_OBJECT_0 != WaitForSingleObject( pi.hProcess, 0 )){
              Thread::yield();
            }
            CloseHandle( pi.hProcess );
            CloseHandle( pi.hThread );
            if( lambda ){
              lambda( 0 );
            }
          });
        }
        return true;
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================