function [value, x] = simplex(A, b, fun)
    [rows, columns] = size(A);
    tab = [A, eye(rows), b];
    fun = cat(1, -fun, zeros(rows + 1, 1));

    tab = cat(1, tab, fun');

    x = zeros(columns, 1);

    [rows, columns] = size(tab);

    pivots = [];

    while true
        % find the minimum entry in the last row (except last column)
        [min_val, piv_c] = min(tab(rows, 1:columns - 1));

        % no negative entries, solution found
        if min_val >= 0
            break;
        end

        % find pivot row
        min_pos = Inf; % Initialize with a large value
        for i = 1:(rows-1)
            % Check if the value is positive and update minimum if necessary
            if tab(i, piv_c) > 0 && tab(i, columns) / tab(i, piv_c) < min_pos
                min_pos = tab(i, columns) / tab(i, piv_c);
                piv_r = i;
            end
        end

        if min_pos == Inf
            disp("Problem is unbounded.");
            x = NaN;
            value = NaN;
            break
        end

        pivots = [pivots, [piv_c piv_r]'];

        pivot = tab(piv_r, piv_c);
        tab(piv_r, :) = tab(piv_r, :) ./ pivot;

        for i=1:rows
            if i ~= piv_r
                tab(i, :) = tab(i, :) - tab(i, piv_c) .* tab(piv_r, :);
            end
        end
    end

    value = tab(rows, columns);
    x(pivots(1, :)) = tab(pivots(2, :), columns);
end