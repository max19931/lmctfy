#ifndef SRC_RESOURCES_FILESYSTEM_RESOURCE_HANDLER_H_
#define SRC_RESOURCES_FILESYSTEM_RESOURCE_HANDLER_H_

#include <memory>
#include <string>
using ::std::string;

#include "base/macros.h"
#include "system_api/kernel_api.h"
#include "lmctfy/controllers/rlimit_controller.h"
#include "lmctfy/resources/cgroup_resource_handler.h"
#include "include/lmctfy.h"
#include "util/errors.h"
#include "util/task/statusor.h"

namespace containers {
namespace lmctfy {

class ContainerSpec;
class ContainerStats;
class EventFdNotifications;

typedef ::system_api::KernelAPI KernelApi;

class FilesystemResourceHandlerFactory : public CgroupResourceHandlerFactory {
 public:
  static ::util::StatusOr<FilesystemResourceHandlerFactory *> New(CgroupFactory *cgroup_factory, const KernelApi *kernel,EventFdNotifications *eventfd_notifications);
  FilesystemResourceHandlerFactory(const RLimitControllerFactory *rlimit_controller_factory,CgroupFactory *cgroup_factory,const KernelApi *kernel);
  virtual ~FilesystemResourceHandlerFactory() {}

 protected:
  virtual ::util::StatusOr<ResourceHandler *> GetResourceHandler(const string &container_name) const;
  virtual ::util::StatusOr<ResourceHandler *> CreateResourceHandler(const string &container_name, const ContainerSpec &spec) const;

 private:
  string GetEffectiveContainerName(const string &container_name) const;
  const ::std::unique_ptr<const RLimitControllerFactory> rlimit_controller_factory_;
  friend class FilesystemResourceHandlerFactoryTest;
  DISALLOW_COPY_AND_ASSIGN(FilesystemResourceHandlerFactory);
};

class FilesystemResourceHandler : public CgroupResourceHandler {
 public:
  FilesystemResourceHandler(const string &container_name,const KernelApi *kernel,RLimitController *rlimit_controller);
  virtual ~FilesystemResourceHandler() {}
  virtual ::util::Status Stats(Container::StatsType type,ContainerStats *output) const;
  virtual ::util::Status Spec(ContainerSpec *spec) const;
  virtual ::util::StatusOr<Container::NotificationId> RegisterNotification(const EventSpec &spec, Callback1< ::util::Status> *callback);

  virtual ::util::Status DoUpdate(const ContainerSpec &spec);
  virtual void RecursiveFillDefaults(ContainerSpec *spec) const;
  virtual ::util::Status VerifyFullSpec(const ContainerSpec &spec) const;

 private:
  RLimitController *rlimit_controller_;

  DISALLOW_COPY_AND_ASSIGN(FilesystemResourceHandler);
};

}  // namespace lmctfy
}  // namespace containers

#endif  // SRC_RESOURCES_FILESYSTEM_RESOURCE_HANDLER_H_
