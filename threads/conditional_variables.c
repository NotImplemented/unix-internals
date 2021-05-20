char buffer[SIZE];
int count = 0, putIndex = 0, getIndex = 0;
struct lock l;
struct condition dataAvailable;
struct condition spaceAvailable;

lock_init(&l);
cond_init(&dataAvailable);
cond_init(&spaceAvailable);


// -----------


void put(char c) {
    lock_acquire(&l);
    count++;
    buffer[putIndex] = c;
    putIndex++;
    if (putIndex == SIZE) {
        putIndex = 0;
    }
    lock_release(&l);
}

char get() {
    char c;
    lock_acquire(&l);
    count--;
    c = buffer[getIndex];
    getIndex++;
    if (getIndex == SIZE) {
        getIndex = 0;
    }
    lock_release(&l);
    return c;
}



// ---------------
// Empty/filled buffer case, waiting using mutexes


void put(char c) {
    lock_acquire(&l);
    while (count == SIZE) {
        lock_release(&l);
        lock_acquire(&l);
    }
    count++;
    buffer[putIndex] = c;
    putIndex++;
    if (putIndex == SIZE) {
        putIndex = 0;
    }
    lock_release(&l);
}

char get() {
    char c;
    lock_acquire(&l);
    while (count == 0) {
        lock_release(&l);
        lock_acquire(&l);
    }
    count--;
    c = buffer[getIndex];
    getIndex++;
    if (getIndex == SIZE) {
        getIndex = 0;
    }
    lock_release(&l);
    return c;
}


// --------------
// Using conditional variables


void put(char c) {
    lock_acquire(&l);
    while (count == SIZE) {
        cond_wait(&spaceAvailable, &l);
    }
    count++;
    buffer[putIndex] = c;
    putIndex++;
    if (putIndex == SIZE) {
        putIndex = 0;
    }
    cond_signal(&dataAvailable, &l);
    lock_release(&l);
}

char get() {
    char c;
    lock_acquire(&l);
    while (count == 0) {
        cond_wait(&dataAvailable, &l);
    }
    count--;
    c = buffer[getIndex];
    getIndex++;
    if (getIndex == SIZE) {
        getIndex = 0;
    }
    cond_signal(&spaceAvailable, &l);
    lock_release(&l);
    return c;
}




