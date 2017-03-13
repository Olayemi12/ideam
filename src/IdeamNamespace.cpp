/*
 * Copyright 2017 A. Mosca <amoscaster@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "IdeamNamespace.h"

#include <Application.h>
#include <AppFileInfo.h>
#include <Roster.h>
#include <StringList.h>

#include "DefaultSettingsKeys.h"
#include "TPreferences.h"


namespace IdeamNames
{

int32
CompareVersion(const BString appVersion, const BString fileVersion)
{
	BStringList appVersionList, fileVersionList;

	if (appVersion == fileVersion)
		return 0;

	appVersion.Split(".", true, appVersionList);
	fileVersion.Split(".", true, fileVersionList);

	// Shoud not happen, but return just in case
	if (appVersionList.CountStrings() != fileVersionList.CountStrings())
		// TODO notify
		return 0;

	for (int32 index = 0; index < appVersionList.CountStrings(); index++) {
		if (appVersionList.StringAt(index) == fileVersionList.StringAt(index))
			continue;
		if (appVersionList.StringAt(index) < fileVersionList.StringAt(index))
			return -1;
		else if (appVersionList.StringAt(index) > fileVersionList.StringAt(index))
			return 1;
	}
	// Should not get here
	return 0;
}

BString
GetSignature()
{
	char signature[B_MIME_TYPE_LENGTH];
	app_info appInfo;

	if (be_app->GetAppInfo(&appInfo) == B_OK) {
		BFile file(&appInfo.ref, B_READ_ONLY);
		if (file.InitCheck() == B_OK) {
			BAppFileInfo appFileInfo(&file);
			if (appFileInfo.InitCheck() == B_OK) {
				appFileInfo.GetSignature(signature);
			}
		}
	}

	return signature;
}

/*
 * retcode: IsEmpty(), "0.0.0.0", version
 */
BString
GetVersionInfo()
{
	BString version("");
	version_info info = {0};
	app_info appInfo;

	if (be_app->GetAppInfo(&appInfo) == B_OK) {
		BFile file(&appInfo.ref, B_READ_ONLY);
		if (file.InitCheck() == B_OK) {
			BAppFileInfo appFileInfo(&file);
			if (appFileInfo.InitCheck() == B_OK) {
				appFileInfo.GetVersionInfo(&info, B_APP_VERSION_KIND);
				version << info.major << "." << info.middle
					<< "." << info.minor << "." << info.internal;
			}
		}
	}

	return version;
}

status_t
UpdateSettingsFile()
{
	status_t status;
	BString stringVal;
	int32	intVal;

	TPreferences settings(kSettingsFileName, kApplicationName, 'IDSE');

	if ((status = settings.InitCheck()) != B_OK)
		return status;

	// General Page
	if (settings.FindString("projects_directory", &stringVal) != B_OK)
		settings.SetBString("projects_directory", kSKProjectsDirectory);
	if (settings.FindInt32("fullpath_title", &intVal) != B_OK)
		settings.SetInt32("fullpath_title", kSKFullPathTitle);
	// General Startup Page
	if (settings.FindInt32("reopen_files", &intVal) != B_OK)
		settings.SetInt32("reopen_files", kSKReopenFiles);
	if (settings.FindInt32("show_projects", &intVal) != B_OK)
		settings.SetInt32("show_projects", kSKShowProjects);
	if (settings.FindInt32("show_output", &intVal) != B_OK)
		settings.SetInt32("show_output", kSKShowOutput);
	if (settings.FindInt32("show_toolbar", &intVal) != B_OK)
		settings.SetInt32("show_toolbar", kSKShowToolBar);
	// Editor Page
	if (settings.FindInt32("edit_fontsize", &intVal) != B_OK)
		settings.SetInt32("edit_fontsize", kSKEditorFontSize);
	if (settings.FindInt32("syntax_highlight", &intVal) != B_OK)
		settings.SetInt32("syntax_highlight", kSKSyntaxHighlight);
	if (settings.FindInt32("tab_width", &intVal) != B_OK)
		settings.SetInt32("tab_width", kSKTabWidth);
	if (settings.FindInt32("brace_match", &intVal) != B_OK)
		settings.SetInt32("brace_match", kSKBraceMatch);
	if (settings.FindInt32("save_caret", &intVal) != B_OK)
		settings.SetInt32("save_caret", kSKSaveCaret);
	// Editor Visual Page
	if (settings.FindInt32("show_linenumber", &intVal) != B_OK)
		settings.SetInt32("show_linenumber", kSKShowLineNumber);
	if (settings.FindInt32("mark_caretline", &intVal) != B_OK)
		settings.SetInt32("mark_caretline", kSKMarkCaretLine);
	if (settings.FindInt32("show_edgeline", &intVal) != B_OK)
		settings.SetInt32("show_edgeline", kSKShowEdgeLine);
	if (settings.FindString("edgeline_column", &stringVal) != B_OK)
//		settings.SetString("edgeline_column", "66");
		settings.SetString("edgeline_column", kSKEdgeLineColumn);
	if (settings.FindInt32("enable_folding", &intVal) != B_OK)
		settings.SetInt32("enable_folding", kSKEnableFolding);
	//  Notifications Page
	if (settings.FindInt32("enable_notifications", &intVal) != B_OK)
		settings.SetInt32("enable_notifications", kSKEnableNotifications);

	// Managed to get here without errors, reset counter and app version
	settings.SetInt64("last_used", real_time_clock());
	// Reset counter
	if ((status = settings.SetInt32("use_count", 0)) != B_OK)
		return status;
	// Reset app version
	return settings.SetBString("app_version", IdeamNames::GetVersionInfo());
}





} // namespace IdeamNames

