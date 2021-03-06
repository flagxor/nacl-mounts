/*
 * Copyright (c) 2011 The Native Client Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that be
 * found in the LICENSE file.
 */
#ifndef PACKAGES_SCRIPTS_FILESYS_MEMORY_MEMNODE_H_
#define PACKAGES_SCRIPTS_FILESYS_MEMORY_MEMNODE_H_

#include <string.h>
#include <sys/stat.h>
#include <list>
#include <string>
#include "../base/Node.h"
#include "MemMount.h"

class MemMount;

// MemNode is the node object for the memory mount (mem_mount class)
// This class overrides all of the MountNode sys call methods.  In
// addition, this class keeps track of parent/child relationships by
// maintaining a parent node pointer and a list of children.
class MemNode : public Node {
 public:
  // constructor initializes the private variables
  MemNode();

  // destructor frees allocated memory
  ~MemNode();

  // Override the sys call methods from mount_node
  int remove();
  int stat(struct stat *buf);
  int access(int amode);
  int chmod(mode_t mode);
  int utime(struct utimbuf const *times);
  int unlink(void);
  int rmdir(void);

  // Add child to this node's children.  This method will do nothing
  // if this node is a directory or if child points to a child that is
  // not in the children list of this node
  void AddChild(MemNode *child);

  // Remove child from this node's children.  This method will do
  // nothing if the node is not a directory
  void RemoveChild(MemNode *child);

  // Reallocate the size of data to be len bytes.  Copies the
  // current data to the reallocated memory.
  void ReallocData(int len);

  // children() returns a list of MemNode pointers
  // which represent the children of this node.
  // If this node is a file or a directory with no children,
  // a NULL pointer is returned.
  std::list<MemNode *> *children(void);

  // set_name() sets the name of this node.  This is not the
  // path but rather the name of the file or directory
  void set_name(std::string name) { name_ = name; }

  // name() returns the name of this node
  std::string name(void) { return name_; }

  // set_parent() sets the parent node of this node to
  // parent_
  void set_parent(MemNode *parent) { parent_ = parent; }

  // parent() returns a pointer to the parent node of
  // this node.  If this node is the root node, NULL
  // is returned.
  MemNode *parent(void) { return parent_; }

  // increase the use count by one
  void IncrementUseCount(void) { ++use_count_; }

  // decrease the use count by one
  void DecrementUseCount(void) { --use_count_; }

  // returns the use count of this node
  int use_count(void) { return use_count_; }

  // capacity() returns the capcity (in bytes) of this node
  int capacity(void) { return capacity_; }

  // set the capacity of this node to capacity bytes
  void set_capacity(int capacity) { capacity_ = capacity; }

  // truncate() sets the length of this node to zero
  void Truncate() { len_ = 0; }

  // data() returns a pointer to the data of this node
  char *data(void) { return data_; }

  // set_data() sets the length of this node to len
  void set_len(size_t len) { len_ = len; }

  // len() returns the length of this node
  size_t len(void) { return len_; }

  // set_mount() sets the mount to which this node belongs
  void set_mount(MemMount *mount) { mount_ = mount; }

  // stat helper
  void raw_stat(struct stat *buf);

 private:
  std::string name_;
  MemNode *parent_;
  MemMount *mount_;
  char *data_;
  size_t len_;
  size_t capacity_;
  int use_count_;
  std::list<MemNode *> children_;
};

#endif  // PACKAGES_SCRIPTS_FILESYS_MEMORY_MEMNODE_H_

