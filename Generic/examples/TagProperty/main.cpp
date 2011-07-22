#include <Generic/LibrarySpool.h>
#include <Generic/TagProperty.h>
#include <Generic/HookT.h>
#include <Generic/Types.h>
#include <libclass/dos.h>
#include <libclass/utility.h>
#include <utility/tagitem.h>


uint StartupFlags = 0;

using namespace GenNS;

class Main
{
   HookT<Main, TagProperty*, TagItem*> hook1, hook2;
   TagProperty prop1, *prop2;

protected:
   uint proc1(TagProperty* prop, TagItem* tags)
   {
      request("info", "PROC1: TagProperty %08lx. Tag Changes Follow", "ok", ARRAY((uint)prop));
      DOS->VPrintf("PROC1: TagProperty %08lx. Tag Changes Follow\n", ARRAY((uint)prop));
      TagItem *tag;
      
      while (0 != (tag = Utility->NextTagItem(&tags)))
      {
         DOS->VPrintf("     : Tag %08lx Data %08lx\n", ARRAY(tag->ti_Tag, tag->ti_Data));
      }

      return 0;
   }

   uint proc2(TagProperty* prop, TagItem* tags)
   {
      DOS->VPrintf("PROC2: TagProperty %08lx. Tag Changes Follow\n", ARRAY((uint)prop));
      TagItem *tag;
      
      while (0 != (tag = Utility->NextTagItem(&tags)))
      {
         DOS->VPrintf("     : Tag %08lx Data %08lx\n", ARRAY(tag->ti_Tag, tag->ti_Data));
      }

      return 0;
   }

protected:
   enum Tags
   {
      Tag_One  = 0x10000,
      Tag_Two,
      Tag_Three,
      Tag_Four
   };

public:
   Main() : 
      prop1(0)
   {
      TagItem tgs1a[] = { {Tag_One, 10}, {Tag_Two, 30}, {TAG_DONE,}};
      TagItem tgs2a[] = { {Tag_Three, 3}, {Tag_Four, 7}, {TAG_DONE,}};
      TagItem tgs1x[] = { {Tag_One, 18}, {Tag_Two, 16}, {TAG_DONE,}};
      TagItem tgs2x[] = { {Tag_Three, 6}, {Tag_Four, 10}, {TAG_DONE,}};

      request("info", "initializing hooks", "ok", 0);
      hook1.Initialize(this, &Main::proc1);
      hook2.Initialize(this, &Main::proc2);

      request("info", "creating prop2 tagproperty with initial tags", "ok", 0);
      prop2 = new TagProperty(tgs2a);

      request("info", "adding prop1 and prop2 listeners", "ok", 0);
      prop1.OnUpdate = hook1;
      prop2->OnUpdate = hook2;

      request("info", "start update sequence for prop2\n", "ok", 0);
      prop2->BeginUpdate();
      request("info", "update prop2 (new values, no info)\n", "ok", 0);
      prop2->Update(tgs2x);
      request("info", "update prop2 (reverting to old values, again no info)\n", "ok", 0);
      prop2->Update(tgs2a);
      request("info", "end update sequence for prop2. should stay silent\n", "ok", 0);
      prop2->EndUpdate();

      request("info", "start update sequence for prop2\n", "ok", 0);
      prop2->BeginUpdate();
      request("info", "update prop2 (new values, no info)\n", "ok", 0);
      prop2->Update(tgs2x);
      request("info", "update prop2 (new values, again no info)\n", "ok", 0);
      prop2->Update(tgs2x);
      request("info", "end update sequence for prop2. should update\n", "ok", 0);
      prop2->EndUpdate();

      request("info", "update prop2 (reverting to old values)\n", "ok", 0);
      prop2->Update(tgs2a);


      request("info", "updating prop1 (no tags are being tracked)", "ok", 0);
      prop1.Update(tgs1x);
      request("info", "updating prop2 (both tags are being tracked)", "ok", 0);
      prop2->Update(tgs2x);

      request("info", "adding tags to prop1", "ok", 0);
      prop1.AddTags(tgs1a);

      request("info", "updating prop1 (both tags are being tracked)", "ok", 0);
      prop1.Update(tgs1x);
      request("info", "updating prop2 (both tags don't change)", "ok", 0);
      prop2->Update(tgs2x);

      request("info", "removing one of the tags from prop2", "ok", 0);
      prop2->RemTags(TAGARRAY(Tag_Four, 0));

      request("info", "updating prop2 (one tag remains)", "ok", 0);
      prop2->Update(tgs2a);

   }

   ~Main()
   {
      request("info", "disposing prop2", "ok", 0);
      delete prop2;
   }
};

int main()
{
   LibrarySpool::Init();
   request("info", "initializing main", "ok", 0);
   delete new Main;
   request("info", "cleaning up", "ok", 0);
   LibrarySpool::Exit();
   return 0;
}
