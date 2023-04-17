// you cannot parallelise it, because a dependency from the previous answer must
// be used in the next computation, and when vectorizing it we cannot take into
// account a difference in every array[i] value when doing 4 things at a time
