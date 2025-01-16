#include "include/buffer_manager.h"
#include <ctime>

int main()
{
    
    std::string filename = "data-5w-50w-zipf.txt";
    FILE* db_file = fopen("data.dbf", "r");
    /* create db with certain size */
    if (db_file == NULL) {
        db_file = fopen("data.dbf", "w");
        void* buffer = malloc(50000 * FRAMESIZE);
        fwrite(buffer, FRAMESIZE, 50000, db_file);
        free(buffer);
    }
    fclose(db_file);

    clock_t time1 = clock();

    BMgr* bmgr = new BMgr("data.dbf");

    FILE* data_file = fopen(filename.c_str(), "r");
    if (data_file == NULL) {
        printf("Error: file %s doesn't exist\n", filename.c_str());
        return 0;
    }

    int is_dirty, page_id;
    while (fscanf(data_file, "%d,%d", &is_dirty, &page_id) != EOF) {
        bmgr->FixPage(page_id, is_dirty);
    }
    clock_t time2 = clock();

    printf("Read Page: %d times, Write Page: %d times.\n", bmgr->ReadNum(), bmgr->WriteNum());
    printf("Buffer Hit Rate: %.2f.\n", bmgr->hitrate());
    printf("Missions Complete in %.2f seconds.\n", (double)(time2-time1) / CLOCKS_PER_SEC);
    fclose(data_file);
    return 0;
}