.PHONY: all clean

TARGET= osca_fs

all: ${TARGET}

osca_fs: osca_fs.o fs_fat.o fs_dentry.o fs_block.o fs_os.o copy_os.o

osca_fs.o: osca_fs.c oscafs.h

fs_fat.o: fs_fat.c oscafs.h

fs_dentry.o: fs_dentry.c oscafs.h

fs_block.o: fs_block.c oscafs.h

fs_os.o: fs_os.c oscafs.h

copy_os.o: copy_os.c oscafs.h

clean:
	rm -f *~
	rm -f *.o
	rm -f ${TARGET}
	rm -f OSCAFS
