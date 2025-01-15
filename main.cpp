#include "include/buffer_manager.h"

int main()
{
    //char* filename = const_cast<char*>(cmd.rest()[0].c_str());
    string filename = "data-5w-50w-zipf.txt";
    FILE* db_file = fopen("data.dbf", "r");
    /* create db with certain size */
    if (db_file == NULL) {
        db_file = fopen("data.dbf", "w");
        void* buffer = malloc(50000 * FRAMESIZE);
        fwrite(buffer, FRAMESIZE, 50000, db_file);
        free(buffer);
    }
    fclose(db_file);
    //printf("dbfile exist.");
    clock_t start_time = clock();
    /* create buffer pool manager */
    BMgr* bmgr = new BMgr("data.dbf", choice);
    /* read data file */
    FILE* data_file = fopen(filename.c_str(), "r");
    if (data_file == NULL) {
        fprintf(stderr, "Error: file %s doesn't exist\n", filename);
        exit(1);
    }
    printf("�ɹ����ļ�...\n");
    int is_dirty, page_id;
    while (fscanf(data_file, "%d,%d", &is_dirty, &page_id) != EOF) {
        bmgr->FixPage(page_id, is_dirty);
        //printf("\rִ������%d", line++);
        //bmgr->UnfixPage(page_id);
    }
    printf("ִ����ϣ�\n���м���: %d, δ���м���: %d, ������%.2f%%\n", bmgr->InBufferCount, bmgr->OutBufferCount, (float)bmgr->InBufferCount * 100.0 / (bmgr->InBufferCount + bmgr->OutBufferCount));
    printf("��ҳ����: %d, дҳ����: %d, IO�ܴ���: %d\n", bmgr->GetIONum(0), bmgr->GetIONum(1), bmgr->GetIONum(0) + bmgr->GetIONum(1));
    printf("����ʱ��: %.2fs\n", (double)(clock() - start_time) / CLOCKS_PER_SEC);
    fclose(data_file);
    system("pause");
    return 0;
}