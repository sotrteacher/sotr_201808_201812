all: shmem_unsemph


shmem_unsemph:shmem_unsemph.c
	gcc shmem_unsemph.c -o shmem_unsemph -lpthread
	./$@


clean:
	rm -v shmem_unsemph
