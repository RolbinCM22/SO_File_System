#pragma once
#include <iostream>
#include <string>
#include "iNode.hpp"

/**
 * @class File
 * @brief Represents a file that encapsules metadata.
 *
 * This class provides essential file operations to perform on the entity,
 * including the metadata retrieval.
 */
class File {
 private:
  /** @brief Metadata structure containing file attributes (iNode). */
  iNode metadata;
 public:
  /** @brief Constructs a new File object with default metadata. */
  File();
  /** @brief Destroys the File object and releases any related resources. */
  ~File();

  /**
   * @brief Retrieves the file name.
   * @return File name as a string.
   */
  std::string getName() const {return this->metadata.name;}

  /**
   * @brief Gets the file type (e.g., directory, regular file, link).
   * @return File type as a string.
   */
  std::string getType() const {return this->metadata.type;}

  /**
  * @brief Gets the current state of the file.
  * @return File state as a string.
  */
  std::string getState() const {}

  /**
  * @brief Gets the length of the file in bytes.
  * @return File length.
  */
  uint64_t getLength() const;

  /**
  * @brief Retrieves the underlying iNode metadata.
  * @return iNode structure containing file attributes.
  */
  inline iNode getMetadata() const {return this->metadata;}
  /**
  * @brief Opens the file for operations.
  */
  void open();
  /**
  * @brief Closes the file and releases associated resources.
  */
  void close();
};