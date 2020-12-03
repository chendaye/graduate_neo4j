package top.chendaye666.domain;

import java.util.List;

/**
 * REVIEWED  影评 person -> movie
 *
 * 概要
 */
public class Summary {
    private final List<String> summary;

    public Summary(List<String> summary) {
        this.summary = summary;
    }

    public List<String> getSummary() {
        return summary;
    }
}
