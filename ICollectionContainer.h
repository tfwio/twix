#ifndef __ICollectionContainer_H__
#define __ICollectionContainer_H__

template<typename tenum>
class ICollectionContainer
{
  public: class ICollection
  {
  public:
    ICollection(){}
    ICollection(tenum toggleIdx, const std::initializer_list<tenum> &d) : collection(d), mParamIdx(toggleIdx) { }
    void Update(IPlugBase *pPlug) { ToggleCollection(pPlug, !pPlug->GetParam(int(mParamIdx))->Bool()); }
    tenum    GetParamIdx() { return mParamIdx; }
    bool IsParamIdx(tenum pIdx) { return mParamIdx == pIdx; }
  private:
    void ToggleCollection(IPlugBase *pPlug, bool pDisable)
    {
      IGraphics *lGui = pPlug->GetGUI();
      for (auto idx = collection.begin(); idx != collection.end(); ++idx)
      {
        int midx = GetControlIndex(pPlug, *idx);
        IControl *mc = lGui->GetControl(midx);
        //lGui->ShowMessageBox(mc->GetParam()->GetNameForHost(), "child", IDOK);
        if (mc) mc->GrayOut(pDisable);
      }
    }
    int GetControlIndex(IPlugBase *pPlug, int mIdx)
    {
      IGraphics *lGui = pPlug->GetGUI();
      for (int i = 0; i < lGui->GetNControls(); ++i)
      {
        if (lGui->GetControl(i)->ParamIdx() == int(mIdx)) {
          //lGui->ShowMessageBox(pPlug->GetParam(mParamIdx)->GetNameForHost(),"parent",IDOK);
          return i;
        }
      }
      return -1;
    }
    std::vector<tenum> collection;
    tenum    mParamIdx;
  };
  //using Collction = std::vector < tenum >;
  using Recollection = std::vector < ICollection >;
public:
  ICollectionContainer(){}
  ICollectionContainer(const std::initializer_list<ICollection> &list) : collection(list) {}
  void refresh(IPlugBase *pPlug) { for (auto i = collection.begin(); i != collection.end(); ++i) i->Update(pPlug); }
  
  void refresh(IPlugBase *pPlug, tenum pParam)
  { for (auto i = collection.begin(); i != collection.end(); ++i) if (i->IsParamIdx(pParam)) i->Update(pPlug); }

private:
  Recollection collection;
};

#endif