##################################################
## CMakeTools externals - Check script
##
## Base for 'check'-scripts
##################################################


##################################################
## Includes
##################################################
include(${CMAKETOOLS_DIR}/External/Base.cmake)


##################################################
## MACRO: external_check_download
##
## Download external package
##################################################
macro(external_check_download dir checksum)
	# Get path with or without / at the end
	set(path "")
	if(NOT "${dir}" STREQUAL "")
		set(path "${dir}/")
	endif()

	# Checksum
	set(md5sum "")
	if(NOT "${checksum}" STREQUAL "")
		set(md5sum "${checksum}")
	endif()

	# Get the download URL
	if (PL_EXTERNAL_USER AND PL_EXTERNAL_PASS)
		# With
		set(url "http://${PL_EXTERNAL_USER}:${PL_EXTERNAL_PASS}@${PL_EXTERNAL_REPOSITORY}/${CMAKETOOLS_CONFIG_NAME}_${CMAKETOOLS_TARGET_ARCH}_${CMAKETOOLS_TARGET_BITSIZE}/${path}${CMAKETOOLS_CURRENT_NAME}.tar.gz")
	else()
		# Without authentication
		set(url "http://${PL_EXTERNAL_REPOSITORY}/${CMAKETOOLS_CONFIG_NAME}_${CMAKETOOLS_TARGET_ARCH}_${CMAKETOOLS_TARGET_BITSIZE}/${path}${CMAKETOOLS_CURRENT_NAME}.tar.gz")
	endif()

	# Download
	external_fetch_http(
		${url}
		${md5sum}
		${CMAKETOOLS_CURRENT_EXT_DIR}/
	)
endmacro(external_check_download dir checksum)

##################################################
## MACRO: external_check_unpack
##
## Unpack external package
##################################################
macro(external_check_unpack)
	# Get filename
	set(filename "${CMAKETOOLS_CURRENT_NAME}.tar.gz")

	# Check if directory exists
	if(NOT EXISTS ${CMAKETOOLS_CURRENT_DEST_DIR})
		# Extract tar archive
		external_extract_tar(TARGZ "${CMAKETOOLS_CURRENT_EXT_DIR}/${filename}" ${CMAKETOOLS_CURRENT_EXT_DIR})
		external_message(STATUS "Archive '${filename}' extracted")
	else()
		# Archive already extracted
		external_message(STATUS "Archive '${filename}' already extracted")
	endif()
endmacro(external_check_unpack)

##################################################
## MACRO: external_check_done
##
## Mark the end of a 'check'-script
##################################################
macro(external_check_done)
	# Create 'done'-file
	file(WRITE ${CMAKETOOLS_CURRENT_BUILD_DIR}/Check.done "done")
endmacro(external_check_done)


##################################################
## Default actions
##################################################

# Remove 'done'-files
file(REMOVE ${CMAKETOOLS_CURRENT_BUILD_DIR}/Check.done)
file(REMOVE ${CMAKETOOLS_CURRENT_BUILD_DIR}/Fetch.done)
file(REMOVE ${CMAKETOOLS_CURRENT_BUILD_DIR}/Configure.done)
file(REMOVE ${CMAKETOOLS_CURRENT_BUILD_DIR}/Build.done)
file(REMOVE ${CMAKETOOLS_CURRENT_BUILD_DIR}/Install.done)

# Set log file
set(CMAKETOOLS_CURRENT_LOG "${CMAKETOOLS_CURRENT_BUILD_DIR}/Log/Check.log")
file(WRITE ${CMAKETOOLS_CURRENT_LOG} "")

# Create source directory
file(MAKE_DIRECTORY "${CMAKETOOLS_CURRENT_EXT_DIR}")
