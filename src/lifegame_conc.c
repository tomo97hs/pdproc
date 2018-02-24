/*
 * The Life Game
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define NLOOP	(200)
#define N	(8192)
#define M	(8192)
#define ALIVE	(1)
#define DEAD	(0)
#define N_THREADS	(8)
#define WIDTH	(2) //ブロック数(横)
#define HEIGHT	(4) //ブロック数(縦)

typedef int Grid[N + 2][M + 2];

typedef struct _ID
{	
	int i0;
	int i1;
	int j0;
	int j1;
} ID;

static Grid g[2];
int cur = 0;

void *computeNextGen(void *arg)
{
	ID id = *(ID *)arg;

	int i, j;
	const Grid *pCur = &g[cur];
	Grid *pNext = &g[(cur + 1) & 1];
	
	for (i = id.i0; i <= id.i1; ++i) {
		for (j = id.j0; j <= id.j1; ++j) {
			int count = 0;

			/* NW neighbor */
			if ((*pCur)[i - 1][j - 1] == ALIVE)
				count++;
			/* N neighbor */
			if ((*pCur)[i - 1][j] == ALIVE)
				count++;
			/* NE neighbor */
			if ((*pCur)[i - 1][j + 1] == ALIVE)
				count++;
			/* W neighbor */
			if ((*pCur)[i][j - 1] == ALIVE)
				count++;
			/* E neighbor */
			if ((*pCur)[i][j + 1] == ALIVE)
				count++;
			/* SW neighbor */
			if ((*pCur)[i + 1][j - 1] == ALIVE)
				count++;
			/* S neighbor */
			if ((*pCur)[i + 1][j] == ALIVE)
				count++;
			/* SE neighbor */
			if ((*pCur)[i + 1][j + 1] == ALIVE)
				count++;

			if (count <= 1 || count >= 4)
				(*pNext)[i][j] = DEAD;
			else if ((*pCur)[i][j] == ALIVE &&
				 (count == 2 || count == 3))
				(*pNext)[i][j] = ALIVE;
			else if ((*pCur)[i][j] == DEAD && count == 3)
				(*pNext)[i][j] = ALIVE;
			else
				(*pNext)[i][j] = DEAD;
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	
	
	int i, j, n, ret, chunk_sizeX, chunk_sizeY, gridIdx[N_THREADS], gridIdy[N_THREADS], a = 1;
	pthread_t th[N_THREADS];
	
	ID id[N_THREADS];
	//th_sqrt = sqrt(N_THREADS);
	//chunk_size = N / th_sqrt;
	
	printf("\033[2J");	/* clear screen */
	for (i = 0; i <= N + 1; ++i) {
		for (j = 0; j <= M + 1; ++j) {
			g[0][i][j] = random() & 1;
		}
		printf("Initializing g[%6d]...\r", i);
	}

	for (n = 0; n < NLOOP; n++) {
		printf("\033[2J");	/* clear screen */
		printf("n = %d\n", n);
		for (i = 1; i <= N >> 8; ++i) { // N >> 8 は 32?
			for (j = 1; j <= M >> 7; ++j) { // M >> 7 は 64? 32*64のグリッド
				printf("%c", g[cur][i][j] == ALIVE ? '@' : '.');
			}
			printf("\n");
		}


		for (i = 1; i <= N_THREADS; ++i) {
			/*
			if (i % th_sqrt == 0) {
				gridIdx = th_sqrt - 1;
				gridIdy = i / th_sqrt - 1;
			} else {
				gridIdx = i % th_sqrt - 1;
				gridIdy = i / th_sqrt;
			}
			*/


			
			if (i % WIDTH == 0) {
				gridIdx[i - 1] = 1;
				gridIdy[i - 1] = i - a - 1;
				a++;
			} else {
				gridIdx[i - 1] = 0;
				gridIdy[i - 1] = i - a;
			}
			
			chunk_sizeX = N / WIDTH;
			chunk_sizeY = N / HEIGHT;

			id[i - 1].i0 = chunk_sizeY * gridIdy[i - 1] + 1;
			id[i - 1].i1 = chunk_sizeY * (gridIdy[i - 1] + 1);
			id[i - 1].j0 = chunk_sizeX * gridIdx[i - 1] + 1;
			id[i - 1].j1 = chunk_sizeX * (gridIdx[i - 1] + 1);
			
			ret = pthread_create(&th[i - 1], NULL, computeNextGen, (void*)&id[i - 1]);
			if (ret != 0)
				perror("new thread");
		}
			
		for (i = 0; i < N_THREADS; ++i)
			pthread_join(th[i], NULL);

		cur = (cur + 1) & 1; // curは0,1,0,1,...と繰り返される
		a = 1;
	}	
	return 0;
}
