
APP_EXEC=WeChat
APP_NAME=$(APP_EXEC).app
APP_PATH=/Applications/$(APP_NAME)/Contents/MacOS
LIBAD_PATH=.
LIBAD_NAME=libantiduck.dylib
BACKUP_NAME=$(APP_EXEC).antiduck.bak
INSERT_DYLIB=./insert_dylib

inject::
	@if ! [[ $EUID -eq 0 ]]; then\
		echo "This script should be run using sudo or as the root user.";\
		exit 1;\
	fi
	@if ! [ -f "${APP_PATH}/${APP_EXEC}" ]; then\
		echo "Can not find the WeChat.";\
		exit 1;\
	fi
	@if ! [ -f "${LIBAD_PATH}/${LIBAD_NAME}" ]; then\
		echo "Can not find the libantiduck, please build first.";\
		exit 1;\
	fi
	@if [ -f "${APP_PATH}/${LIBAD_NAME}" ]; then\
		rm -f "${APP_PATH}/${LIBAD_NAME}";\
		cp "${LIBAD_PATH}/${LIBAD_NAME}" "${APP_PATH}";\
		echo "Library found! Replace with new libantiduck successfully!";\
	else \
		cp "${APP_PATH}/${APP_EXEC}" "${APP_PATH}/${BACKUP_NAME}";\
		cp "${LIBAD_PATH}/${LIBAD_NAME}" "${APP_PATH}";\
		"${INSERT_DYLIB}" "@executable_path/${LIBAD_NAME}" "${APP_PATH}/${APP_EXEC}" "${APP_PATH}/${APP_EXEC}" --all-yes;\
		codesign --force --deep --sign - "/Applications/${APP_NAME}";\
		echo "Inject successfully!";\
	fi

restore::
	@if ! [[ $EUID -eq 0 ]]; then\
		echo "This script should be run using sudo or as the root user.";\
		exit 1;\
	fi
	@if ! [ -f "${APP_PATH}/${APP_EXEC}" ]; then\
		echo "Can not find the WeChat.";\
		exit 1;\
	fi
	@if ! [ -f "${APP_PATH}/${BACKUP_NAME}" ]; then\
		echo "Can not find the WeChat backup file.";\
		exit 1;\
	fi
	@rm -rf "${APP_PATH}/${LIBAD_NAME}";
	@mv "${APP_PATH}/${BACKUP_NAME}" "${APP_PATH}/${APP_EXEC}";
	@echo "Restore successfully";

