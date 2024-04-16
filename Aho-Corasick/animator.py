"""A simple animator for the Aho-Corasick algorithm.
Only suffix links and states are animated.
Built to help create materials for teaching of the algorithm."""

import logging
import manim

ALPHABET = 26


class AhoCorasick:
    """Implementation of Aho-Corasick algorithm for string matching.
    Details of algorithm outlined at https://dl.acm.org/doi/pdf/10.1145/360825.360855.
    Trie nodes are referenced using array indices to make animation easier."""

    class Node:
        """Trie node"""

        nxt: list[int]
        """List of pointers to children nodes on the Trie, default to sentinel value -1."""

        go: list[int]
        """Aho-Corasick's automaton."""

        suffix_link, exit_link = -1, -1
        """Pointer to the "suffix link" and "exit link", default to sentinel value -1."""

        leaf_indices: list[int]
        """List of indices of strings that end at this node."""

        def __init__(self):
            self.nxt = [-1] * ALPHABET
            self.go = [-1] * ALPHABET
            self.suffix_link, self.exit_link = -1, -1
            self.leaf_indices = []

    trie: list[Node]
    """List of nodes representing the Trie. The first node is the root node."""

    def __init__(self, patterns: list[str]):
        """Initialize the Trie with a list of patterns."""

        self.trie = [self.Node()]
        for i, pattern in enumerate(patterns):
            self._insert_string(pattern, i)
        self._build_links()

    def _insert_string(self, s: str, idx: int):
        """Insert a string into the Trie."""

        ptr = 0  # Start at the Trie's root
        for c in s:
            c = ord(c) - ord('a')
            if self.trie[ptr].nxt[c] == -1:
                self.trie[ptr].nxt[c] = len(self.trie)
                self.trie.append(self.Node())
            ptr = self.trie[ptr].nxt[c]
        self.trie[ptr].leaf_indices.append(idx)

    def _build_links(self):
        """Build suffix links and exit links using a Breadth-First Search."""
        queue, queue_ptr = [0], 0
        while queue_ptr < len(queue):  # We may modify the queue during each iteration
            v = queue[queue_ptr]
            queue_ptr += 1

            # The suffix link
            sf_v = self.trie[v].suffix_link

            # Build the exit link
            if v != 0:  # v is not root
                if len(self.trie[sf_v].leaf_indices):
                    self.trie[v].exit_link = sf_v
                else:
                    self.trie[v].exit_link = self.trie[sf_v].exit_link

            for c in range(ALPHABET):
                sf_child = 0 if v == 0 else self.trie[sf_v].go[c]
                if self.trie[v].nxt[c] != -1:
                    # A child exists with label c
                    child = self.trie[v].nxt[c]

                    self.trie[v].go[c] = child
                    self.trie[child].suffix_link = sf_child
                    queue.append(child)
                else:
                    self.trie[v].go[c] = sf_child

    def dfs(self, left: float, right: float, depth: int, idx: int, result: list[tuple[int, float, list[int, str]]]):
        """Split the width range of a node with index idx into equal parts and assign them to its children
        with a Depth-First Search. Assigns (depth, x coord, children) in list result."""

        children = [(cidx, chr(ord('a') + c))
                    for c, cidx in enumerate(self.trie[idx].nxt) if cidx != -1]
        if len(children):
            children_width = (right - left) / len(children)
            for i, (cidx, _) in enumerate(children):
                self.dfs(left + i * children_width, left + (i + 1)
                         * children_width, depth + 1, cidx, result)

        result[idx] = depth, (left + right)/2, children


PIXEL_HEIGHT, PIXEL_WIDTH = 2160, 2160
HEIGHT, WIDTH = 10, 10
RADIUS = 0.3
TRIE_WIDTH = 7
TRIE_ORIGIN = manim.UP * HEIGHT / 2 + manim.LEFT * WIDTH / 2
TEXT_HEIGHT = 5
PATTERN_HEIGHT = 6.3


class AhoCorasickAnimate(manim.Scene):
    def _construct_trie(self):
        def generate_tree():
            node_data = [None] * len(self.ac.trie)
            self.ac.dfs(0, TRIE_WIDTH, 0, 0, node_data)

            max_depth = max([depth for depth, _, _ in node_data])
            row_height = HEIGHT / (max_depth+1)

            for idx, (depth, x, children) in enumerate(node_data):
                y = (depth + 0.5) * row_height
                node_data[idx] = (x, y, children)

            return node_data

        node_data = generate_tree()

        # Circles that represents nodes
        nodes, node_labels = [None] * len(node_data), [None] * len(node_data)
        for idx, (x, y, _) in enumerate(node_data):
            nodes[idx] = manim.Circle(radius=RADIUS, z_index=1).move_to(
                x * manim.RIGHT + y * manim.DOWN + TRIE_ORIGIN)
            node_labels[idx] = manim.Text(
                str(idx), font_size=85*RADIUS).move_to(nodes[idx])

        self.play(*[manim.Create(obj) for obj in nodes + node_labels])

        # Lines that represent forward links
        forward_edges = []
        for idx, (_, _, children) in enumerate(node_data):
            for child_idx, label in children:
                edge = manim.LabeledLine(label, font_size=85*RADIUS, buff=0, z_index=0,
                                         start=nodes[idx].get_center(
                                         ) + manim.DOWN * RADIUS,
                                         end=nodes[child_idx].get_center() + manim.UP * RADIUS)
                forward_edges.append(edge)

        self.play(*[manim.Create(obj) for obj in forward_edges])

        # # Lines that represent suffix links
        # suffix_edges = []
        # for idx in range(len(node_data)):
        #     sf_v = self.ac.trie[idx].suffix_link
        #     if sf_v == -1: continue
        #     suffix_edges.append(manim.Line(
        #         nodes[idx].get_center() + manim.UP * RADIUS, nodes[sf_v].get_center() + manim.DOWN * RADIUS,
        #         color=manim.RED
        #     ))

        suffix_tags = []
        for idx in range(len(node_data)):
            sf_v = self.ac.trie[idx].suffix_link
            suffix_tags.append(manim.Text(str(sf_v), font_size=70*RADIUS).move_to(
                nodes[idx].get_center() + manim.RIGHT * RADIUS * 1.8))

        leaf_tags = []
        for idx in range(len(node_data)):
            leaf_rep = str([v+1 for v in self.ac.trie[idx].leaf_indices]
                           ) if len(self.ac.trie[idx].leaf_indices) else ""
            leaf_tags.append(manim.Text(leaf_rep, font_size=70*RADIUS).move_to(
                suffix_tags[idx].get_center() + manim.DOWN * RADIUS * 1))

        # self.play(*[manim.Create(obj) for obj in suffix_edges])
        self.play(*[manim.Create(obj) for obj in suffix_tags] +
                  [manim.Create(obj) for obj in leaf_tags])

        return nodes, suffix_tags, leaf_tags

    def _draw_text(self):
        YUP = (HEIGHT / 2 - 1) * manim.UP
        YDOWN = YUP + manim.DOWN * TEXT_HEIGHT
        YSTEP = (YDOWN - YUP) / len(self.text)
        XCOORD = (WIDTH / 2 - 2) * manim.RIGHT

        text = [manim.Text(str(c), font_size=85*RADIUS).move_to(
            YUP + YSTEP * idx + XCOORD) for idx, c in enumerate(self.text)]
        result = [[[], manim.MathTex(r"\varnothing", font_size=90*RADIUS).move_to(
            character.get_center() + manim.RIGHT * RADIUS * 3)] for character in text]

        self.play(*[manim.Create(obj) for obj in text] +
                  [manim.Create(obj[1]) for obj in result])

        return text, result

    def _draw_pattern(self):
        patterns = manim.VGroup(
            *[manim.Text(f"{idx+1}. {pattern}", font_size=85*RADIUS) for idx, pattern in enumerate(self.patterns)])
        patterns.move_to((PATTERN_HEIGHT - HEIGHT/2) *
                         manim.DOWN + (WIDTH / 2 - 2) * manim.RIGHT)
        patterns.arrange(direction=manim.DOWN, center=False,
                         aligned_edge=manim.LEFT)

        self.play(*[manim.Create(obj) for obj in patterns])
        return patterns

    def construct(self):
        nodes, suffix_tags, leaf_tags = self._construct_trie()

        ptr = 0
        circle_ptr = manim.Circle(radius=RADIUS, color=manim.BLUE, z_index=2).move_to(
            nodes[ptr].get_center())
        self.play(manim.Create(circle_ptr))

        def move_ptr(nptr: int):
            nonlocal ptr
            ptr = nptr
            if ptr == -1:
                circle_ptr.generate_target().move_to(
                    nodes[0].get_center() + manim.UP * 3)
            else:
                circle_ptr.generate_target().move_to(nodes[ptr].get_center())
            self.play(manim.MoveToTarget(circle_ptr))

        text, result = self._draw_text()
        text_ptr = manim.Arrow(text[0].get_center() + manim.LEFT * RADIUS * 3,
                               text[0].get_center() + manim.LEFT * RADIUS * 1, buff=0, color=manim.GREEN)
        self.play(manim.Create(text_ptr))

        def move_text_ptr(idx: int):
            text_ptr.generate_target().move_to(
                text[idx].get_center() + manim.LEFT * RADIUS * 2)
            self.play(manim.MoveToTarget(text_ptr))

        patterns = self._draw_pattern()

        def wiggle(obj: manim.Mobject):
            return manim.Wiggle(obj, run_time=0.5, scale_value=1.5)

        for idx, c in enumerate(self.text):
            move_text_ptr(idx)
            c = ord(c) - ord('a')

            while ptr != -1 and self.ac.trie[ptr].nxt[c] == -1:
                self.play(wiggle(suffix_tags[ptr]))
                move_ptr(self.ac.trie[ptr].suffix_link)
            move_ptr(0 if ptr == -1 else self.ac.trie[ptr].nxt[c])

            eptr = ptr
            eptr_arrow = manim.Arrow(nodes[eptr].get_center() + manim.LEFT * RADIUS * 3.5,
                                     nodes[eptr].get_center() + manim.LEFT * RADIUS * 1.5, buff=0, color=manim.BLUE)
            self.play(manim.Create(eptr_arrow))

            while eptr != -1:
                if len(self.ac.trie[eptr].leaf_indices):
                    result[idx][0] += self.ac.trie[eptr].leaf_indices
                    target_text = ", ".join([str(v+1) for v in result[idx][0]])
                    temp_leaf_tag = leaf_tags[eptr].copy()
                    result[idx][1].target = temp_leaf_tag.target = manim.Text(
                        target_text, font_size=85*RADIUS).move_to(text[idx].get_center() + manim.RIGHT * RADIUS * 3)
                    
                    self.play(manim.MoveToTarget(result[idx][1]), manim.MoveToTarget(temp_leaf_tag),
                              *[wiggle(patterns[v]) for v in self.ac.trie[eptr].leaf_indices])
                    self.remove(temp_leaf_tag)
                    temp_leaf_tag

                self.play(wiggle(suffix_tags[eptr]))
                eptr = self.ac.trie[eptr].suffix_link
                if eptr != -1:
                    eptr_arrow.generate_target().move_to(
                        nodes[eptr].get_center() + manim.LEFT * RADIUS * 2.5)
                else:
                    eptr_arrow.generate_target().move_to(
                        nodes[0].get_center() + manim.UP * 3 + manim.LEFT * RADIUS * 2.5)
                self.play(manim.MoveToTarget(eptr_arrow))
        self.wait(2)

    def __init__(self, text: str, patterns: list[str]):
        super().__init__()
        self.text = text
        self.patterns = patterns
        self.ac = AhoCorasick(patterns)


# Direct execution of Manim.
# Referenced https://gist.github.com/alxpettit/98ea885adc287c23ae2cd390fe5cb7e0.
if __name__ == '__main__':  # Just making sure that this wasn't imported
    manim.config.pixel_height, manim.config.pixel_width = PIXEL_HEIGHT, PIXEL_WIDTH
    manim.config.frame_height, manim.config.frame_width = HEIGHT, WIDTH
    manim.config.frame_rate = 100

    T = input("Enter the text to search in (T): ")
    N = int(input("Enter the number of patterns (n): "))
    P = [input(f"Enter pattern {i+1}: ") for i in range(N)]

    logging.info("Building the Aho-Corasick automaton.")
    ac = AhoCorasickAnimate(T, P)
    logging.info("Rendering the scene.")
    ac.render(True)
