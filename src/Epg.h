#ifndef TVTEST_EPG_H
#define TVTEST_EPG_H


#include "ProgramGuide.h"
#include "Favorites.h"


namespace TVTest
{

	class CEpg
	{
	public:
		class CChannelProviderManager : public CProgramGuideChannelProviderManager
		{
		public:
			CChannelProviderManager();
			~CChannelProviderManager();
		// CProgramGuideChannelProviderManager
			size_t GetChannelProviderCount() const override;
			CProgramGuideChannelProvider *GetChannelProvider(size_t Index) const override;
		// CChannelProviderManager
			bool Create(LPCTSTR pszDefaultTuner=NULL);
			void Clear();
			int GetCurChannelProvider() const { return m_CurChannelProvider; }

		private:
			class CBonDriverChannelProvider : public CProgramGuideBaseChannelProvider
			{
			public:
				CBonDriverChannelProvider(LPCTSTR pszFileName);
				bool Update() override;
			};

			class CFavoritesChannelProvider : public CProgramGuideBaseChannelProvider
			{
			public:
				~CFavoritesChannelProvider();
				bool Update() override;
				bool GetName(LPTSTR pszName,int MaxName) const override;
				bool GetGroupID(size_t Group,String *pID) const override;
				int ParseGroupID(LPCTSTR pszID) const override;
				bool GetBonDriver(LPTSTR pszFileName,int MaxLength) const override;
				bool GetBonDriverFileName(size_t Group,size_t Channel,LPTSTR pszFileName,int MaxLength) const override;

			private:
				struct GroupInfo
				{
					String Name;
					String ID;
					std::vector<CFavoriteChannel> ChannelList;
				};

				std::vector<GroupInfo*> m_GroupList;

				void ClearGroupList();
				void AddFavoritesChannels(const CFavoriteFolder &Folder,const String &Path);
				void AddSubItems(GroupInfo *pGroup,const CFavoriteFolder &Folder);
		};

			std::vector<CProgramGuideChannelProvider*> m_ChannelProviderList;
			int m_CurChannelProvider;
		};

		CProgramGuide ProgramGuide;
		CProgramGuideFrameSettings ProgramGuideFrameSettings;
		CProgramGuideFrame ProgramGuideFrame;
		CProgramGuideDisplay ProgramGuideDisplay;
		bool fShowProgramGuide;

		CEpg(LibISDB::EPGDatabase &EPGDatabase,CEventSearchOptions &EventSearchOptions);
		CChannelProviderManager *CreateChannelProviderManager(LPCTSTR pszDefaultTuner=NULL);

	private:
		class CProgramGuideEventHandler : public CProgramGuide::CEventHandler
		{
			bool OnClose() override;
			void OnDestroy() override;
			void OnServiceTitleLButtonDown(LPCTSTR pszDriverFileName,const LibISDB::EPGDatabase::ServiceInfo *pServiceInfo) override;
			bool OnKeyDown(UINT KeyCode,UINT Flags) override;
			bool OnMenuInitialize(HMENU hmenu,UINT CommandBase) override;
			bool OnMenuSelected(UINT Command) override;

			int FindChannel(const CChannelList *pChannelList,const LibISDB::EPGDatabase::ServiceInfo *pServiceInfo);
		};

		class CProgramGuideDisplayEventHandler
			: public CProgramGuideDisplay::CProgramGuideDisplayEventHandler
			, protected CDisplayEventHandlerBase
		{
		// CProgramGuideDisplay::CProgramGuideDisplayEventHandler
			bool OnHide() override;
			bool SetAlwaysOnTop(bool fTop) override;
			bool GetAlwaysOnTop() const override;
			void OnMouseMessage(UINT Msg,int x,int y) override;
		};

		class CProgramGuideProgramCustomizer : public CProgramGuide::CProgramCustomizer
		{
			bool Initialize() override;
			void Finalize() override;
			bool DrawBackground(const LibISDB::EventInfo &Event,HDC hdc,
				const RECT &ItemRect,const RECT &TitleRect,const RECT &ContentRect,
				COLORREF BackgroundColor) override;
			bool InitializeMenu(const LibISDB::EventInfo &Event,HMENU hmenu,UINT CommandBase,
								const POINT &CursorPos,const RECT &ItemRect) override;
			bool ProcessMenu(const LibISDB::EventInfo &Event,UINT Command) override;
			bool OnLButtonDoubleClick(const LibISDB::EventInfo &Event,
									  const POINT &CursorPos,const RECT &ItemRect) override;
		};

		CProgramGuideEventHandler m_ProgramGuideEventHandler;
		CProgramGuideDisplayEventHandler m_ProgramGuideDisplayEventHandler;
		CProgramGuideProgramCustomizer m_ProgramCustomizer;
		CChannelProviderManager m_ChannelProviderManager;
	};

}	// namespace TVTest


#endif
