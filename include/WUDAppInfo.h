#pragma once

#include <adwaita.h>
#include <string>

class WUDAppInfo {

public:
    AppInfo();
    const std::string &getId() const;
    void setId(const std::string &id);

    const std::string &getName() const;
    void setName(const std::string &name);

    const std::string &getShortName() const;
    void setShortName(const std::string &shortName);

    const std::string &getDescription() const;
    void setDescription(const std::string &description);

    const std::string &getVersion() const;
    void setVersion(const std::string &version);

    const std::string &getChangelog() const;
    void setChangelog(const std::string &changelog);

    const std::string &getGitHubRepo() const;
    void setGitHubRepo(const std::string &gitHubRepo);

    const std::string &getIssueTracker() const;
    void setIssueTracker(const std::string &issueTracker);

    const std::string &getSupportUrl() const;
    void setSupportUrl(const std::string &supportUrl);

private:
    std::string id;
    std::string name;
    std::string shortName;
    std::string description;
    std::string version;
    std::string changelog;
    std::string githubRepo;
    std::string issueTracker;
    std::string supportUrl;
};