#pragma once

#include <engine/core/base/echo_def.h>
#include <vector>
#include <string>
#include <QTreeView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

using namespace std;

namespace QT_UI
{
	//---------------------------------------------
	// DirectoryModel for Qt  2012-6-3 帝林
	//---------------------------------------------
	class QDirectoryModel : public QStandardItemModel
	{
		Q_OBJECT

		typedef std::map<string, QIcon>  IconMap;

	public:
		// Root
		struct RootPath
		{
			string		m_display;
			string		m_path;
			bool		m_expand;
		};
		typedef std::vector<RootPath> RootPathArray;

	public:
		// 构造函数
		QDirectoryModel();

		// 设置主目录, 文件类型过滤
		void setRootPath(const RootPathArray& rootPaths, const char* extFilter, QTreeView* treeView, QSortFilterProxyModel* proxy);

		// 是否支持文件类型
		bool IsSupportExt( const string& ext);

		// 刷新显示
		void Refresh() { Refresh(NULL); }
		void Refresh(volatile bool* interrupt);

		// 设置图标
		void SetIcon( const char* name, const QIcon& icon){ m_iconMaps[name] = icon; }

		// 设置当前选中
		void setCurrentSelect( const char* dir);

		// 根据鼠标位置获取目录
		QString getFileUnderMousePos(const QPoint& pos);

		// 清空
		void Clean();

	private:
		// 迭代目录
		void RecursiveDir( const string& dir, QStandardItem* parentItem) { RecursiveDir(dir, parentItem, NULL); }
		void RecursiveDir( const string& dir, QStandardItem* parentItem, volatile bool* interrupt);

		// 获取文件图标
		QIcon getFileIcon( const char* fullPath);

		void _addToRingQueue(QStandardItem* parent, QStandardItem* child);

	public: signals:
		// 编辑材质
		void FileSelected( const char* path);

		// 编辑文件
		void FileEdit( const char* path);

		void signalThreadFindDirectory();

	private slots:
		// 展开某Fiter
		void OnExpandedFilter(const QModelIndex& pIndex);

		// 选择某文件
		void onSelectedFile( const QModelIndex& pIndex);

		// 双击某文件
		void OnEditFile( const QModelIndex& pIndex);

	private:
		QTreeView*						m_treeView = nullptr;	// treeView
		QSortFilterProxyModel*			m_proxy = nullptr;	    // proxy
		RootPathArray					m_rootPaths;
		vector<string>					m_exts;			        // 文件类型
		IconMap							m_iconMaps;		        // 图标
		QStandardItem*					m_activeItem = nullptr;	// active item
		std::vector< QStandardItem*>	m_dirItems;		        // 所有目录item
		QModelIndex						m_currentSelect;        // 当前选中
		Echo::Dword						m_selectTime = 0;	    // 当前选中时间
	};
}
