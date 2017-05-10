#ifndef __IPopupMenuButton_h__
#define __IPopupMenuButton_h__

class PresetPopupButton : public IPanelControl
{
  typedef std::vector<std::string> svec;
public:
  PresetPopupButton(IPlugBase *pPlug, IRECT pR, IBitmap *pBmp, const char *pFileFilter=".ds")
    : IPanelControl(pPlug, pR, &COLOR_RED)
    , mPreviousPath("")
    , mLibraryPath("c:/users/xo/desktop/library")
    , mMenuDir()
    , m_fileFilter(pFileFilter)
  {
    do_build();
  }
private:

  void OnMouseDown(int x, int y, IMouseMod* pMod)
  {
    if (pMod->L)
    {
      doPopupMenu();
    }
  }

  void do_build(){
    IPopupMenuItem *child;
    IGraphics* gui = mPlug->GetGUI();
    // [0-2]
    mMenuRoot.AddItem("Save Program...");
    mMenuRoot.AddItem("Save Bank...");
    mMenuRoot.AddSeparator();
    // [3-5]
    mMenuRoot.AddItem("Load Program...");
    mMenuRoot.AddItem("Load Bank...");
    mMenuRoot.AddSeparator();
    // [6-7]
    mMenuRoot.AddItem("Load DsPreset...");
    mMenuRoot.AddSeparator();
    // [8]
    mMenuRoot.AddItem("Library-Location");
    dir_menu(mLibraryPath.Get());
    child = mMenuRoot.AddItem("Library", 9, &mMenuDir);

  }

  // Createe directory structure in local `IPopupMenu mDir`
  void dir_menu(std::string fpath)
  {
    hasError = false;
    on::fs::get_pathinfo(fpath, m_fileFilter, dirs, files);
    int id = 0;

    if (dirs.size()) for (auto item = dirs.begin(); item != dirs.end(); ++item)
    {
      //WDL_String path(item->c_str());
      //path = WDL_String(path.remove_filepart());
      //path = WDL_String(path.remove_fileext());
      CreateNode(&mMenuDir, (*item));
    }
  }

  // looking for a particular index
  void Find(int index, IPopupMenu &pMnu, IPopupMenuItem *pRef, int &n)
  {
    for (int i = 0; i < pMnu.GetNItems(); ++i, ++n)
    {
      
    }
  }
  // looking for a particular index
  void Find(int index, IPopupMenuItem &item, IPopupMenuItem *pRef, int &n)
  {
    Find(index, *item.GetSubmenu(), pRef, n);
  }

  // Populate pParent menu with file-items
  void CreateNode(IPopupMenu *pParent, std::string &fpath)
  {
    std::string dirname = fpath;
    svec lFiles = svec();
    on::fs::get_pathinfo_p(fpath, ".ds", 0, &lFiles);

    IPopupMenuItem *thisItem = pParent->AddItem(fpath.c_str(), lFiles.size() ? new IPopupMenu() : (IPopupMenu*)0);
    IPopupMenu *sub = thisItem->GetSubmenu();

    if (sub) {
      sub->SetChosenItemIdx(-1);
      for (auto item = lFiles.begin(); item != lFiles.end(); ++item)
      {
        sub->AddItem((*item).c_str());
        //process_dir(&mDir, mDir.AddItem((*item).c_str()/*, id++*/), (*item));
      }
    }
    //for (auto item = lFiles.begin(); item != lFiles.end(); ++item)
    //{
    //  process_dir(&mDir, mDir.AddItem((*item).c_str()/*, id++*/), (*item));
    //}
  }

  // Show the menu and find its selection-index
  void doPopupMenu()
  {
    mMenuRoot.SetChosenItemIdx(-1);
    mMenuRoot.SetHandleMessage(false);
    MSG msg;
    int nHi = -1, nLo = -1;
    if (GetGUI()->CreateIPopupMenu(&mMenuRoot, &mRECT))
    {
      if (PeekMessage(&msg, ((IGraphicsWin*)GetGUI())->GetMainWnd(), WM_COMMAND, WM_COMMAND, PM_REMOVE))
      {
        nHi = HIWORD(msg.wParam);
        nLo = LOWORD(msg.wParam);

      }
      int itemChosen = mMenuRoot.GetChosenItemIdx();

      WDL_String fileName;
      //printf("chosen %i /n", itemChosen);
      switch (itemChosen)
      {
      case -1: {
        std::string ws(dirs[0].c_str());
        ws.append(hasError ? " error" : " good");
        IPopupMenuItem *mn = mMenuRoot.GetItem(9);
        IPopupMenu *sb = mn->GetSubmenu();
        int new_selected = sb->GetChosenItemIdx();
        bool _isset;

        for (int i = 0; i < sb->GetNItems(); i++)
        {
          if (sb->GetItem(i)->GetChecked())
          {
            //int idxx = sb->GetChosenItemIdx();
            GetGUI()->ShowMessageBox(std::to_string(i).c_str(), ws.c_str(), IDOK);
            _isset = true;
            i = sb->GetNItems();
          }
        }
        if (!_isset)
        {
          std::string itemc = "NOTSET lo: ";
          itemc.append(std::to_string(nLo));
          itemc.append(", hi: ");
          itemc.append(std::to_string(nHi));
          itemc.append(", lp-lo: ");
          itemc.append(std::to_string(LOWORD(msg.lParam)));
          itemc.append(", lp-hi: ");
          itemc.append(std::to_string(HIWORD(msg.lParam)));
          GetGUI()->ShowMessageBox(itemc.c_str(), "NOTSET Some other selectedItem", IDOK);
          //GetGUI()->ShowMessageBox("Some other selectedItem", itemc.c_str(), IDOK);
        }
        else
        {
          std::string itemc = "NOTSET lo: ";
          itemc.append(std::to_string(nLo));
          itemc.append(", hi: ");
          itemc.append(std::to_string(nHi));
          itemc.append(", lp-lo: ");
          itemc.append(std::to_string(LOWORD(msg.lParam)));
          itemc.append(", lp-hi: ");
          itemc.append(std::to_string(HIWORD(msg.lParam)));
          GetGUI()->ShowMessageBox(itemc.c_str(), "Some other selectedItem", IDOK);
        }
        //delete mn;
        //delete sb;
      }
               break;
      case 0: //Save Program
        fileName.Set(mPlug->GetPresetName(mPlug->GetCurrentPresetIdx()));
        GetGUI()->PromptForFile(&fileName, kFileSave, &mPreviousPath, "fxp");
        mPlug->SaveProgramAsFXP(&fileName);
        break;
      case 1: //Save Bank
        fileName.Set("a16_chunksBank");
        GetGUI()->PromptForFile(&fileName, kFileSave, &mPreviousPath, "fxb");
        mPlug->SaveBankAsFXB(&fileName);
        break;
      case 3: //Load Preset
        GetGUI()->PromptForFile(&fileName, kFileOpen, &mPreviousPath, "fxp");
        mPlug->LoadProgramFromFXP(&fileName);
        break;
      case 4: // Load Bank
        GetGUI()->PromptForFile(&fileName, kFileOpen, &mPreviousPath, "fxb");
        mPlug->LoadBankFromFXB(&fileName);
        break;
      case 6: // Try to load a DS file.
        GetGUI()->PromptForFile(&fileName, kFileOpen, &mPreviousPath, "ds");
        //mPlug->LoadBankFromFXB(&fileName);
        break;
      case 9:
        if (dirs.size())
        {
          std::string ws(dirs[0].c_str());
          ws.append(hasError ? " error" : " good");
          IPopupMenuItem *mn = mMenuRoot.GetItem(itemChosen);
          IPopupMenu *sb = mn->GetSubmenu();

          for (int i = 0; i < sb->GetNItems(); i++)
          {
            if (sb->GetItem(i)->GetChecked())
            {
              //int idxx = sb->GetChosenItemIdx();
              GetGUI()->ShowMessageBox(std::to_string(i).c_str(), ws.c_str(), IDOK);
            }
          }

          delete mn;
          delete sb;
        }
        break;
      default:
        if (true)//(itemChosen >= 0)
        {
          fileName.Append("\n menu-item: ");
          fileName.Append((char*)std::to_string(itemChosen).c_str());
          this->GetGUI()->ShowMessageBox(fileName.Get(), "", IDOK);
        }
      }
    }
  }
  const char* m_fileFilter;
  bool hasError;
  svec dirs, files;
  IPopupMenu mMenuRoot, mMenuDir;
  WDL_String mPreviousPath;
  WDL_String mLibraryPath;
};

#endif